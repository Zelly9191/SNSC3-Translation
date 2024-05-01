#pragma once
#include "globals.h"
#include "TextNode.h"
using namespace std;

class JEScript
{
public:
	//JEScript(string filename);
	string filename;
	//list<unique_ptr<TextNode>> nodes;
	list<TextNode*> nodes;

	void createJEScript();
};

