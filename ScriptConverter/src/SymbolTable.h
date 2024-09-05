#pragma once
#include "globals.h"
#include <map>
using namespace std;


class SymbolTable
{
public:
	SymbolTable();

	map<string, string> TAGS;

	string replaceTags(string line);

private:
	void loadTAGS();
};

extern SymbolTable ST;

