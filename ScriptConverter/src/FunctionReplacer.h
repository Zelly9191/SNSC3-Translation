#pragma once
#include "globals.h"
#include <map>
using namespace std;

class FunctionReplacer
{
public:
	static list<string> replaceFunctions(list<string> text);

private:
	static map<string, list<string>> analyzeFunctions(list<string> text);
};

