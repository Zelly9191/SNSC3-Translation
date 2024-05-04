from copy import deepcopy
import html
import io
import os
import glob
import csv
import argparse
from xml.etree.ElementTree import Element
import lxml.etree as ET
from lxml import html as ETHtml
from JEScripts_Preprocessor import (
    JEScript_CleanupInlineTags,
    JEScript_PreprocessInlineTags,
    Preprocess_JEScript_XML_Content,
)
from pathlib import Path

# -----
# setup argparse
parser = argparse.ArgumentParser(
    description="Find places in .xml scripts (JE Scripts) where <ascii></ascii> translation for gendered (male/female) text is placed not inside them. Then correct them automatically."
)
parser.add_argument(
    "-dir",
    type=str,
    required=True,
    help="Path to folder(s) containing XML files to check and fix (It will be recursively scanned).",
)

parser.add_argument("-v", action="store_true", help="use if you need more debug info")
parser.add_argument("-exclude", nargs="+", type=str, default=[])
parser.add_argument(
    "-AutoClose",
    action="store_true",
    help="use if you want to make all inline tags properly self closing. For example: <end_line> => <end_line/>",
)

args = parser.parse_args()


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


def AddMissingChildAscii(tagList):
    changes_count: int = 0
    if len(tagList) > 0:
        for gender_tag in tagList:
            ascii_tag = gender_tag.find("ascii")
            sibling_ascii_tag = gender_tag.find("../ascii")
            if ascii_tag is None and sibling_ascii_tag is not None:
                gender_tag.append(deepcopy(sibling_ascii_tag))
                changes_count += 1

    return changes_count


def CorrectMaleFemaleTanslations(xmlFile):
    xml_contents: str = ""
    xml_bytes: bytes = 0
    changes_count: int = 0
    with open(xmlFile, "r", encoding="utf-8") as fobj:
        file_contents = fobj.read()
        if len(file_contents) == 0:
            print(f"Error correcting file: {xmlFile}. File is empty. skipping...")
            return

        xml_contents = JEScript_PreprocessInlineTags(file_contents)
        xml_bytes = xml_contents.encode("utf-8")

        try:
            root = ETHtml.fromstring(xml_contents)
            maleTags = root.xpath(".//male")
            femaleTags = root.xpath(".//female")

            changes_count += AddMissingChildAscii(maleTags)
            changes_count += AddMissingChildAscii(femaleTags)
            # now we need to cleanup the badly placed <male>/<female> sibling <ascii> tags
            if changes_count > 0:
                for tag in maleTags:
                    sibling_ascii_tag = tag.find("../ascii")
                    if sibling_ascii_tag is not None:
                        outer_tag = tag.getparent()
                        outer_tag.remove(sibling_ascii_tag)
        except ET.XMLSyntaxError as e:
            if args.v:
                print(
                    f"XmlSyntaxError while parsing file:[{xmlFile}] :: Reason:{e.msg}"
                )

        fobj.close()

    # saving modified xml
    if changes_count > 0:
        print(f"there were some changes. Now we need to resave file: {xmlFile}")
        with open(xmlFile, "w", encoding="UTF-8") as fobj:
            """
            We are iterating over root's children to trim any unnecessary tags (<span></span>) added by lxml introduced when doing: root = ETHtml.fromstring(xml_contents)
            This also help with keeping original formatting.
            """
            for child in root:
                # we need to make sure that it will use the same indent as the file uses
                ET.indent(child, space="  ")
                xml_contents = ETHtml.tostring(child).decode("UTF-8")
                xml_contents = JEScript_CleanupInlineTags(xml_contents, args.AutoClose)
                xml_bytes = html.unescape(xml_contents)
                fobj.write(xml_bytes)
            fobj.close()


def main():
    # parse path that needs to be scanned
    dir = args.dir
    scanPath = Path(dir)  # by default let's assume it's absolute
    if not scanPath.is_absolute():  # now we check if maybe it's a relative path
        # convert relative Path to absolute path for sanity
        scanPath = Path(os.getcwd(), dir).resolve()

    else:
        print("using absolute path")

    # search for xml files:
    print("Scanning dir: [" + args.dir + "] for .xml files")
    # use this for testing and debugging
    XmlList = []
    XmlList.append("./Tests/test2.xml")
    # use this for real list from directory
    # XmlList = Get_file_list(args.dir, ".xml")
    # search through file list

    print(f"searching for missing EN translations in {len(XmlList)} files.")
    print("Please wait...")
    for xml in XmlList:
        CorrectMaleFemaleTanslations(xml)


# End main()

if __name__ == "__main__":
    main()
