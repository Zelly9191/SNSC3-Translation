#pragma once
#include "globals.h"
#include "TextNode.h"

class MenuTextNode : public TextNode
{
public:
	using TextNode::TextNode;
	list<string> GetText() override;
};

