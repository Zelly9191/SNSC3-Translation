#pragma once
#include "globals.h"
#include <map>
using namespace std;


class SymbolTable
{
public:
	SymbolTable();

	map<string, string> tagsTOiscript;
	map<string, string> sjisOnlyTags;

	void replaceTags(string& line, bool ascii);
	void replaceSjisOnlyTags(string& line);

private:
	void loadTAGS();
	void loadSJISONLYTAGS();
};

extern SymbolTable ST;
