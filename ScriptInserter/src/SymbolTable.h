#pragma once
#include "globals.h"
#include <map>
using namespace std;


class SymbolTable
{
public:
	SymbolTable();

	map<string, string> tagsTOiscript;
};
