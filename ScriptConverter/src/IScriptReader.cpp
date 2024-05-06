#include "IScriptReader.h"

IScriptReader::IScriptReader(filesystem::path filename)
{
	IScriptReader::filename = filename.string();
	jeScript.filename = "./" + filename.filename().stem().string() + ".xml";
}

void IScriptReader::readIScript()
{
	ifstream stream(filename);
	string text;

	string textType;
	list<TextLine> sjisText;

	while (getline(stream, text))
	{
		// Line with dialog
		if (!regex_match(text, regex(ENRegex)))
		{
			// position of first quote
			size_t quote1 = text.find("\"");
			if (quote1 == string::npos)
				continue;
			// position of second quote
			size_t quote2 = text.find("\"", quote1 + 1);

			size_t firstSpace = text.find_first_of(" \t");
			textType = text.substr(0, firstSpace);

			// text in between quotes
			string weirdTxt = text.substr(quote1 + 1, quote2 - quote1 - 1);

			TextLine tl(textType, weirdTxt);
			sjisText.push_back(tl);
		}
		// else if there is no dialog on this line
		else
		{
			// Inserts ascii text before copying over the non-dialog lines
			if (!sjisText.empty())
			{
				createTextNodes(sjisText);
				sjisText.clear();
			}
		}
	}
	stream.close();
}

void IScriptReader::createTextNodes(list<TextLine> textLines)
{
	string firstTextType = textLines.front().textType;
	
	if (TextNode::DialogTypes.find(firstTextType) == TextNode::DialogTypes.end())
	{
		cout << firstTextType << " is not a valid text type\n";
		cout << "skipping " << textLines.size() << " lines\n\n";
		return;
	}

	// cant do switch statement with strings :(
	if (firstTextType == "menutitle")
	{
		jeScript.nodes.push_back(new MenuTextNode(textLines));
	}
	else if (firstTextType == "menutxt")
	{
		jeScript.nodes.push_back(new MenuTextNode(textLines));
	}
	else if(firstTextType == "choicetxt")
	{
		jeScript.nodes.push_back(new MenuTextNode(textLines));
	}
	else
	{
		jeScript.nodes.push_back(new TextNode(textLines));
	}
}
