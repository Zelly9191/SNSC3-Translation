#include "JEObject.h"


JEObject::JEObject(list<string> sjisText)
{
	JEObject::sjisText = sjisText;
}

void JEObject::formatText()
{
	// Replace tags in sjis text so it can match iscript text
	for (auto &i : sjisText)
	{
		removeLeadingSpace(i);
		removeEndline(i);
		ST.replaceTags(i, false);
		ST.replaceSjisOnlyTags(i);
	}

	// Replace tags in ascii text to match iscript format
	for (auto &j : asciiText)
	{
		removeLeadingSpace(j);
		removeEndline(j);
		ST.replaceTags(j, true);
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
