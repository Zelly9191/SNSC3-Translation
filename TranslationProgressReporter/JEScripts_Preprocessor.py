import re

faulty_tags = [
 #simple <tag_name> format tags
    "end_line",
    "three_dots",
    "player_name",
    "player_nickname",
    "hearth",
    "paw"
    "weapon_type",
    "break", #<- this one seems to not be present in any .xml(JE scripts). But it was defined in ScriptValidator tool, so we should keep it for safety/compatibility sake (even if it's not needed)
    "quote",
    "music_note",
    "option_1",
    "option_2",
    "lesser_than",
    "greater_than",
    "black_dot",
    "left_arrow",
 #tags with attributes format: <tag_name attr="val"...>
    "player",
    "info",
    "portrait_l",
    "portrait_r",
 #this one can be both: with attributes and simple <tag_name> only formats
    "partner",
]


def Preprocess_JEScript_XML_Content(xml_text: str=""):
    if len(xml_text) > 0:
        for tag_name in faulty_tags:
            xml_text = re.sub(rf'<{tag_name}\b([^>]*)>', rf'<{tag_name}\1/>', xml_text)
    else:
        print("[Preprocess_JEScript_XML_Content()] xml contents where empty. No parsing done")
    return xml_text

#when doing debug test run
if __name__ == '__main__':
    # Load the XML file
    xml_text = ""
    with open("./Tests/test.xml", encoding="utf-8") as fobj:
        xml_text = Preprocess_JEScript_XML_Content(fobj.read())

    # Save the entire XML content (including original contents) to a new file
    with open("./Tests/corrected_parseAsPlainText.xml", "w", encoding="utf-8") as f:
        f.write(xml_text)