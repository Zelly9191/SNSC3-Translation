#include "Validator.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#define strtk_no_tr1_or_boost
#include <strtk.hpp>
#include <regex>

const string XMLdir = "./JE Files";
const string TXTdir = "./IScripts";

const string ENGRegexPrefix = "^[ \t]*[(0-9A-Za-z.,!?=:;'\\-\"*~　 )";
const string JPRegexPrefix = "^[ \t]*[(0-9ぁ-んァ-ヶ一-龥々ー?!？！、，:;\\-\"゠゠*~　 )";
string whitelistTags = "(<three_dots>)*(<player_name>)*(<hearth>)*(<option_1>)*(<option_2>)*(<root>)*(<player_nickname>)*(<hearth>)*(<paw>)*(<partner>)*(<weapon_type>)*(<break>)*(<quote>)*(<music_note>)*(<lesser_than>)*(<greater_than>)*(<black_dot>)*(<left_arrow>)*";
const string regexSuffix = "]+<end_line>$";
const string ideographicSpace = "　";

const string reportArg = "-r";

int main(int argc, char *argv[]) {

    string report = "";
    bool makeReport;
    for (int i = 0; i < argc; i++) {
        if (argv[i] == reportArg) {
            makeReport = true;
        }
    }

    for (const auto& entry : filesystem::directory_iterator(TXTdir)) {
        string fileName = entry.path().filename().string(); // .txt filename with extension (E.g. 17bcb2c.txt)
        string fileIdentifier = entry.path().stem().string(); // .txt filename without extension  (E.g. 17bcb2c)
        findXML(fileName, fileIdentifier, &report);
    }
    std::ofstream file("Report.txt");
    file << report;
    return 0;
}

void findXML(string fileName, string fileIdentifier, string *report) {
    bool xmlFound = false;
    for (const auto& entry : filesystem::directory_iterator(XMLdir)) {
        string XMLName = entry.path().filename().string(); // .xml filename with extension (E.g. 023_25057292_17e580c.xml)
        if (XMLName.find(fileIdentifier) != string::npos) {
            gatherData(fileName, XMLName, report);
            xmlFound = true;
        }
    }
    if (!xmlFound) {
        *report += "Cannot find corresponding XML File for " + fileName + ".\n\n";
        // cerr << "Cannot find corresponding XML File for " << fileName << ".\n\n"; 
    }
}


void gatherData(string txtFile, string xmlFile, string *report) {
    // cout << "Beginning validation for " << xmlFile << endl;
    bool fileValid = validateXML(xmlFile, report);
    if (fileValid) {
       //cout << "File valid, no errors found.\n";
    } else {
        //cerr << "Errors found in " << xmlFile << endl;;
    }
}

bool validateXML(string xmlFile, string *report) {
    ifstream xmlStream(XMLdir + "/" + xmlFile);
    string currentLine;
    int lineCount = 0;
    bool noErrors = true;
    bool locationTag = false;
    if (!xmlStream.is_open()) {
        cerr << "Corresponding .xml file does not exist or cannot be opened.\n";
        noErrors = false;
        return noErrors;
    }

    while (getline(xmlStream, currentLine)) {
        lineCount++;
        //cout << currentLine << endl;
        int JPlines = 0;

        if (currentLine.find("<location>") != string::npos) {
            locationTag = true;
        }
        if (currentLine.find("<sjis>") != string::npos) {
            checkLines(&xmlStream, currentLine, "<sjis>", &lineCount, &noErrors, report, &locationTag);
        }
        if (currentLine.find("<ascii>") != string::npos) {
            checkLines(&xmlStream, currentLine, "<ascii>", &lineCount, &noErrors, report, &locationTag);
        }
        locationTag = false;
    }
    xmlStream.close();
    if (!noErrors) {
        *report += "The above errors are for the file: " + xmlFile + ".\n------------------------\n";
    }
    return noErrors;
}

void checkLines(ifstream *xmlStream, string currentLine, string tagType, int *lineCount, bool *noErrors, string *report, bool *locationTag) {
    int lines = 0;
    string endTag = tagType;
    endTag.insert(1, "/");
    while (getline(*xmlStream, currentLine)) { // Go through lines in the text block
        (*lineCount)++;

        if (currentLine.find(endTag) == string::npos) {
            string prefix = (tagType == "<sjis>") ? JPRegexPrefix : ENGRegexPrefix;
            if (!regex_match(currentLine, regex(prefix + whitelistTags + regexSuffix))) {\
                *report += "ERROR: Incorrect match at line " + to_string(*lineCount) + ".\n";
                //cerr << "Incorrect match at line " << *lineCount << "." << endl;
                *noErrors = false;
            }
            if ((currentLine.find("　") == string::npos) && *locationTag) {
                *report += "Missing ideographic space in location tag.\n";
                *noErrors = false;
            }
            lines++;
        } else {
            break;
        }

        if (lines > 3) {
            *report += "Possible error detected! More than 3 lines of " + tagType + "text on line " + to_string(*lineCount) + ". Check for missing tags or overflowing text.\n";
            //cerr << "Possible error detected! More than 3 lines of " << tagType << "text on line " << *lineCount << ". Check for missing tags or overflowing text.\n";
            *noErrors = false;
            break;
        }
    }

    if (lines == 0) {
        *report += "Empty " + tagType + " text at line " + to_string(*lineCount) + ".\n";
        // cerr << "Empty " << tagType << " text at line " << *lineCount << "\n";
        *noErrors = false;
    } else if (lines <= 3) {
        // cout << "Amount of JP lines: " << JPlines << endl;
    }
}