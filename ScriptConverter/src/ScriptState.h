#pragma once
#include "globals.h"
using namespace std;

class Info
{
public:
	bool left = false;
	bool right = false;
	bool nobox = false;
	bool simplebox = false;
	int effect = -1;
};

class Portrait
{
public:
	bool active = false;
	// for if the speaker has no portrait. happens sometimes.
	int name = -1;

	int entry = -1;
	int eye = -1;
	int mouth = -1;
};

class ScriptState
{
public:
	ScriptState();
	ScriptState(int gender, int partner);
	void Reset();
	bool ungendered();
	bool unpartnered();

	// Used to identify Script State
	string label;
	int lineNumber;

	string textType;
	Info info;
	Portrait portrait_l;
	Portrait portrait_r;
	list<string> sjis;

	bool male = false;
	bool female = false;
	bool rundor = false;
	bool enzi = false;
	bool killfith = false;
	bool rufeel = false;
};
