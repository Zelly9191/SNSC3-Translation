#include "MenuTextNode.h"

list<string> MenuTextNode::GetText()
{
	int tabLevel = 0;

	list<string> text;

	list<string> menuTextType =
	{
		"menutitle",
		"menutxt"
	};

	string head = "";

	if (textLines.front().textType.find("choice"))
		head = "<choice>";
	else
		head = "<menu>";

	text.push_back(head);
	tabLevel += 1;

	for (auto const& i : textLines)
	{
		list<string> txt = TextNode::GetText({ i }, tabLevel);
		text.insert(text.end(), txt.begin(), txt.end());
	}

	text.push_back(head.insert(1, "/"));
	tabLevel += 1;

	return text;
}
