#include "IScriptReader.h"


IScriptReader::IScriptReader(string filename)
{
	IScriptReader::filename = filename;
}

void IScriptReader::readFile()
{
	ifstream file(filename);
	string text;

    int prevLineNumber = -1;
    int lineNumber = 0;

	while (getline(file, text))
	{
        lineNumber++;
        if (text == "")
        {
            // Account for addition of extra lines in other untranslated iscripts
            lineNumber++;
            continue;
        }
        // This line shows up sometimes. It has quotes, but is not dialog.
        if (text.find("strlen") != string::npos)
            continue;
        if (!regex_match(text, regex(ENRegex)))
        {
            // position of first quote
            size_t quote1 = text.find("\"");
            if (quote1 == string::npos)
                continue;
            // position of second quote
            size_t quote2 = text.find("\"", quote1 + 1);
            // quote2 - quote1 - 1 is the difference between the position of quote1 and quote2 minus one for the quote(") itself
            string weirdTxt = text.substr(quote1 + 1, quote2 - quote1 - 1);            

            if (lineNumber == prevLineNumber + 1)
            {
                // Append last IscriptObject
                IScriptObject &lastObj = IScriptObjects.back();
                lastObj.sjisText.push_back(weirdTxt);
            }
            else // Add new IScriptObject
            {
                list<string> txts;
                txts.push_back(weirdTxt);
                IScriptObject iObj = IScriptObject(lineNumber, txts);
                IScriptObjects.push_back(iObj);
            }

            prevLineNumber = lineNumber;
        }
	}

	file.close();
}
