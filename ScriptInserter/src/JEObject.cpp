#include "JEObject.h"


JEObject::JEObject(SymbolTable symTab, list<string> sjisText)
{
	st = symTab;
	JEObject::sjisText = sjisText;
}

void JEObject::formatText()
{
	// Replace tags in sjis text so it can match iscript text
	for (auto &i : sjisText)
	{
		removeLeadingSpace(i);
		removeEndline(i);
		replaceTags(i, false);
	}

	// Replace tags in ascii text to match iscript format
	for (auto &j : asciiText)
	{
		removeLeadingSpace(j);
		removeEndline(j);
		replaceTags(j, true);
	}
}

void JEObject::removeLeadingSpace(string &line)
{
	size_t endlinePos = line.find_first_not_of(" \t");
	line = line.substr(endlinePos);
}

void JEObject::removeEndline(string &line)
{
	size_t endlinePos = line.find("<end_line>");
	line = line.substr(0, endlinePos);
}

void JEObject::replaceTags(string &line, bool ascii)
{
	bool quoteSwitch = true;
	for (auto const& i : st.tagsTOiscript)
	{
		string tag = i.first;

		size_t pos = line.find(tag);
		// Loop handles having multiple of the same tag on a single line
		while (pos != string::npos)
		{
			size_t tagSize = tag.size();

			// EDGE CASES
			
			// <three_dots> has different sjis and ascii format
			if (tag == "<three_dots>" && ascii)
			{
				line = line.substr(0, pos) + "..." + line.substr(pos + tagSize);
			}
			// There are two different quote characters despite only one tag
			else if (tag == "<quote>")
			{
				string quotes = i.second;
				size_t pos2 = quotes.find(",");
				string replacement;
				if (quoteSwitch)
				{
					replacement = quotes.substr(0, pos2);
				}
				else
				{
					replacement = quotes.substr(pos2 + 1);
				}
				line = line.substr(0, pos) + replacement + line.substr(pos + tagSize);
				quoteSwitch = !quoteSwitch;
			}
			// Replaces tags with valid iscript text
			else
				line = line.substr(0, pos) + i.second + line.substr(pos + tagSize);

			pos = line.find(tag);
		}
	}
}
