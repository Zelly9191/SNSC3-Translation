#pragma once
#include "ScriptState.h"
#include <map>

class JEScriptNode
{
public:
	JEScriptNode() { tabLevel = 0; };
	JEScriptNode(ScriptState ss);
	list<string> AllText;

	void addFemale(ScriptState ss);
	void makeOptionNode(ScriptState ss, string headName);

private:
	void makeJEText();
	void makeInfo(Info info);
	void makePortraits(Portrait p_l, Portrait p_r);
	void makeSjis(list<string> sjis);

	int tabLevel;
	ScriptState state;
	static map<int, string> Characters;
	static map<int, string> Eyes;
	static map<int, string> Mouths;
	static map<int, string> Names;
};

