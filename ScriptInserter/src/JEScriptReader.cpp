#include "JEScriptReader.h"

JEScriptReader::JEScriptReader(string filename)
{
    JEScriptReader::filename = filename;
}

void JEScriptReader::readFile()
{
    ifstream file(filename);
    string text;

    bool readingSJIS = false;
    bool readingASCII = false;

    list<string> texts;

    while (getline(file, text))
    {
        // Handles whether it is reading sjis or ascii
        if (text.find("<sjis>") != string::npos)
        {
            readingSJIS = true;
            continue;
        }
        else if (text.find("<ascii>") != string::npos)
        {
            readingASCII = true;
            continue;
        }
        // Makes/alters JEObject when done reading text lines
        else if (text.find("</sjis>") != string::npos)
        {
            readingSJIS = false;
            JEObject jeObj(texts);
            JEObjects.push_back(jeObj);
            texts.clear();
            continue;
        }
        else if (text.find("</ascii>") != string::npos)
        {
            readingASCII = false;
            JEObject &lastObj = JEObjects.back();
            lastObj.asciiText = texts;
            texts.clear();
            continue;
        }
                
        if (readingSJIS || readingASCII)
        {
            texts.push_back(text);
        }
    }

    file.close();
}

void JEScriptReader::formatText()
{
    for (auto &i : JEObjects)
    {
        i.formatText();
    }
}
