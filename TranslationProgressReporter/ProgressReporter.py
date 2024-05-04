import os
import glob
import csv
import argparse
import lxml.etree as ET
from lxml import html as ETHtml
from JEScripts_Preprocessor import Preprocess_JEScript_XML_Content
from pathlib import Path

# -----
# setup argparse
parser = argparse.ArgumentParser(
    description="Find places in .xml scripts (JE Scripts) that are missing English translations"
)
parser.add_argument(
    "-dir",
    type=str,
    required=True,
    help="Path to folder(s) containing XML files to validate (It will be recursively scanned).",
)
parser.add_argument(
    "-out",
    nargs="?",
    default="TranslationReport.csv",
    help="a .csv file where the report should be written to.",
)
parser.add_argument("-v", action="store_true", help="use if you need more debug info")
parser.add_argument("-exclude", nargs="+", type=str, default=[])
parser.add_argument(
    "-skip_gendered",
    action="store_true",
    help="use if you want to skip reporting missing translations inside of <male>/<female> tags",
)
parser.add_argument(
    "-skip_locations",
    action="store_true",
    help="use if you want to skip reporting missing translations inside <location> tags",
)
parser.add_argument(
    "-skip_others",
    action="store_true",
    help="use if you want to skip all other tags with untranslated text",
)


args = parser.parse_args()
# TODO: group error msg in a dictionary, for better filtering and sorting posibilities
# EntriesByCategory = {}
# -----


def Check_skip_dirs(fPath: str = ""):
    if len(fPath) == 0:
        # print("Empty path")
        return True
    # print(f"amount of skipdirs: {len(args.exclude)}")
    if len(args.exclude) > 0:
        for badDir in args.exclude:
            if fPath.find(badDir) > -1:
                print(f"skipping: {fPath}")
                return True

    return False


# end Check_skip_dirs()


def Get_file_list(aDir, aFiletype=".xml"):
    outList = []
    if args.v:
        print(f"Main dir: {aDir}")

    for fPath, subDirs, files in os.walk(aDir):
        for f in files:
            if f.endswith(aFiletype):
                fullpath = os.path.join(fPath, f)
                if Check_skip_dirs(fullpath):
                    continue

                if args.v:
                    print("found [.xml] script file: " + fullpath)
                outList.append(fullpath)

    return outList


# end Get_file_list()


def check_gendered_missing_EN_loc(fName, outer):
    endline_num = outer.sourceline + (len(ETHtml.tostring(outer).strip().split()) - 1)
    nextOuterSibling = outer.getnext()
    outer_tag_name = outer.tag.lower()
    msg = ""
    # if the outer's next sibling is not another gendered tag or a translation, that means the whole JP text was not translated
    if nextOuterSibling is not None and nextOuterSibling.tag == "ascii":
        msg = resultMsg = (
            f"file: [{fName}] :: start Line: {outer.sourceline} -> End line: {endline_num} | [MALE/FEMALE] there is only a single EN translation for both gender's text (<ascii></ascii> tags)."
        )
    # if the outer's next sibling is not another gendered tag or a translation, that means the whole JP text was not translated
    elif nextOuterSibling is None or (
        nextOuterSibling.tag != "male"
        and nextOuterSibling.tag != "female"
        and nextOuterSibling.tag != "ascii"
    ):
        msg = resultMsg = (
            f"file: [{fName}] :: start Line: {outer.sourceline} -> End line: {endline_num} | [MALE/FEMALE] Missing EN translation (<ascii></ascii> tags)."
        )
    return msg


# end check_gendered_missing_EN_loc(inSjisTag)


def find_missing_EN_loc(xmlFile):
    missingEnList = []
    with open(xmlFile, encoding="utf-8") as fobj:
        file_contents = fobj.read()
        if len(file_contents) > 0:
            xml_contents = Preprocess_JEScript_XML_Content(
                file_contents
            )  # here we fixup all unclosed custom tags, to make sure our xml has a valid structure
            xml_bytes = xml_contents.encode("utf-8")
            try:
                root = ETHtml.fromstring(xml_bytes)

                for sjis_tag in root.xpath(".//sjis"):
                    next_sibling = sjis_tag.getnext()
                    if (
                        next_sibling is None or next_sibling.tag != "ascii"
                    ):  # here we know that there is no translation right after JP text in <sjis></sjis>
                        resultMsg = ""
                        endline_num = sjis_tag.sourceline + (
                            len(ETHtml.tostring(sjis_tag).strip().split()) - 1
                        )
                        # now let's check if it's not a <sjis></sjis> wrapped with <male>/<female>tags
                        outer_tag = sjis_tag.getparent()
                        outer_tag_name = outer_tag.tag.lower()
                        if outer_tag_name == "male" or outer_tag_name == "female":
                            if args.skip_gendered:  # check if we should skip gendered
                                continue
                            # gendered text may have a single translation <ascii></ascii> tag, right after </male> or </female> closing tags. This means both <sjis> JP text have the same EN <ascii> translation
                            resultMsg = check_gendered_missing_EN_loc(
                                xmlFile, outer_tag
                            )
                        elif outer_tag_name == "location":
                            if args.skip_locations:  # check if we should skip locations
                                continue
                            type = f"[{outer_tag_name.upper()}]"
                            resultMsg = f"file: [{xmlFile}] :: start Line: {sjis_tag.sourceline} -> End line: {endline_num} | {type} Missing EN translation (<ascii></ascii> tags)."
                        else:
                            if args.skip_others:  # check if we should skip dialogues
                                continue
                            type = f"[{outer_tag_name.upper()}]"
                            resultMsg = f"file: [{xmlFile}] :: start Line: {sjis_tag.sourceline} -> End line: {endline_num} | {type} Missing EN translation (<ascii></ascii> tags)."
                        # save result message
                        if resultMsg != "":
                            missingEnList.append(resultMsg)
                            if args.v:
                                print(resultMsg)
            except ET.XMLSyntaxError as e:
                errorMsg = (
                    f"XmlSyntaxError while parsing file:[{xmlFile}] :: Reason:{e.msg}"
                )
                missingEnList.append(errorMsg)
                if args.v:
                    print(errorMsg)
    return missingEnList


# END find_missing_EN_loc(xmlFile)


def export_to_csv(msgList):
    # TODO: write an export function
    pass


# end export_to_csv(msgList)


def main():
    # parse path that needs to be scanned
    dir = args.dir
    scanPath = Path(dir)  # by default let's assume it's absolute
    if not scanPath.is_absolute():  # now we check if maybe it's a relative path
        scanPath = Path(
            os.getcwd(), dir
        ).resolve()  # convert relative Path to absolute path for sanity
    else:
        print("using absolute path")

    # search for xml files:
    print("Scanning dir: [" + args.dir + "] for .xml files")
    XmlList = Get_file_list(args.dir, ".xml")
    # search through file list
    resultList = []

    print(f"searching for missing EN translations in {len(XmlList)} files.")
    print("Please wait...")
    for xml in XmlList:
        resultList += find_missing_EN_loc(xml)
    finalList = []
    msg = f"Found {len(resultList)} missing EN translations."
    if len(resultList) > 0:
        finalList.append(msg + " Below are the results:")
    else:
        finalList.append(msg)
    print(msg + f" Results are written to: {args.out}")
    finalList += resultList
    # TODO: save xmlList to a .log or to a .csv file
    with open(args.out, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        for item in finalList:
            writer.writerow([item])


# End main()

if __name__ == "__main__":
    main()
