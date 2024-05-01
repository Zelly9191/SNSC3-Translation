#include "TextNode.h"

const map<string,string> TextNode::DialogTypes = {
	{"placetxt","location"},
	{"dialogtxt","dialogue"},
	{"dialogbig","bigtext"},
	{"popuptxt","popup"},
	{"menutitle","title"},
	{"menutxt","option"},
	{"choicetxt","option"}
};

TextNode::TextNode(TextLine textLine)
{
	TextNode::textLines = { textLine };
}

TextNode::TextNode(list<TextLine> textLines)
{
	TextNode::textLines = textLines;
}

int TextNode::validTextType()
{
	string tt = textLines.front().textType;

	int counter = 0;
	for (auto const& i : textLines)
	{
		if (i.textType != tt)
			return counter;
		counter++;
	}
	return -1;
}

list<string> TextNode::GetText()
{
	/*
	if (validTextType() != -1)
	{
		//throw ("Problem separating text types");
		cerr << "Problem separating text types\n";
		return {};
	}	
	*/

	int tabLevel = 0;
	return GetText(textLines, tabLevel);
}

list<string> TextNode::GetText(list<TextLine> lines, int& tabLevel)
{
	list<string> text;
	string textType = lines.front().textType;
	string dType = DialogTypes.at(textType);

	string tab(tabLevel, '\t');
	text.push_back(tab + "<" + dType + ">");
	tabLevel += 1;

	list<string> sjisText = GetSjisText(lines, tabLevel);
	text.insert(text.end(), sjisText.begin(), sjisText.end());
	tabLevel -= 1;

	tab = string(tabLevel, '\t');
	text.push_back(tab + "</" + dType + ">");

	return text;
}

list<string> TextNode::GetSjisText(list<TextLine> lines, int &tabLevel)
{
	list<string> text;

	string tab(tabLevel, '\t');
	text.push_back(tab + "<sjis>");
	tabLevel += 1;

	tab = string(tabLevel, '\t');
	for (auto const& i : lines)
	{
		text.push_back(tab + i.line + "<end_line>");
	}
	tabLevel -= 1;

	tab = string(tabLevel, '\t');
	text.push_back(tab + "</sjis>");

	return text;
}

TextLine::TextLine(string textType, string line)
{
	TextLine::textType = textType;
	TextLine::line = line;
}
