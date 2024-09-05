#include "ScriptState.h"

ScriptState::ScriptState()
{
	label = "";
	lineNumber = -1;
	male = true;
	female = true;
	rundor = true;
	enzi = true;
	killfith = true;
	rufeel = true;
}

ScriptState::ScriptState(int gender, int partner)
{
	label = "";
	lineNumber = -1;
	sjis = list<string>();
	
	switch (gender)
	{
	case 0:
		male = true;
		break;
	case 1:
		female = true;
		break;
	default:
		male = true;
		female = true;
		break;
	}

	switch (partner)
	{
	case 0:
		rundor = true;
		break;
	case 1:
		enzi = true;
		break;
	case 2:
		killfith = true;
		break;
	case 3:
		rufeel = true;
		break;
	default:
		rundor = true;
		enzi = true;
		killfith = true;
		rufeel = true;
		break;
	}
}

void ScriptState::Reset()
{
	textType = "";
	info = Info();
	portrait_l = Portrait();
	portrait_r = Portrait();
	sjis.clear();
}

bool ScriptState::ungendered()
{
	// Return true if lines are read by both male and female characters
	return male && female;
}

bool ScriptState::unpartnered()
{
	// Return true if lines are read by all partners
	return rundor && enzi && killfith && rufeel;
}
