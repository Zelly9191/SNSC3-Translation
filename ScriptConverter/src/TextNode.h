#pragma once
#include "globals.h"
#include <map>
#include "TextLine.h"
using namespace std;

class TextNode
{
public:
	TextNode(TextLine textLine);
	TextNode(list<TextLine> textLines);
	list<TextLine> textLines;
	string info;
	string portrait_l;
	string portrait_r;

	// Checks if textType is the same for all lines.
	// Returns -1 if so, otherwise returns index where textType is different.
	int validTextType();
	virtual list<string> GetText();

	static const map<string, string> DialogTypes;
protected:
	list<string> GetText(list<TextLine> lines, int& tabLevel);
	list<string> GetSjisText(list<TextLine> lines, int &tabLevel);	
};



