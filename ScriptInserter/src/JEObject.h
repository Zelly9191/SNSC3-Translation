#pragma once
#include "globals.h"
using namespace std;

class JEObject
{
public:
	list<string> sjisText;
	list<string> asciiText;

	// replace tags
	void formatText();
};
