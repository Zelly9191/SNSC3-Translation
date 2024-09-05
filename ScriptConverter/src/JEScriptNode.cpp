#include "JEScriptNode.h"
#include "SymbolTable.h"

map<int, string> JEScriptNode::Characters = {
	{ 0, "player" },
	{ 2, "partner" },
	{ 6, "murno" },
	{ 7, "murno_shady" },
	{ 8, "murno_hatless" },
	{ 9, "vee" },
	{ 10, "jade" },
	{ 11, "tier" },
	{ 12, "lemmy" },
	{ 13, "anise" },
	{ 14, "rob" },
	{ 15, "bostaph" },
	{ 16, "velworen" },
	{ 17, "thus" },
	{ 18, "roche" },
	{ 19, "wellman" },
	{ 20, "gallahan" },
	{ 21, "zakk" },
	{ 22, "weido_1" },
	{ 23, "veirdo_2" },
	{ 24, "pike" },
	{ 25, "guilan" },
	{ 26, "soldier" },
	{ 27, "vee_romantic" },
	{ 28, "elieze" },
	{ 29, "tram" },
	{ 30, "benson" },
	{ 31, "lanna" },
};
map<int, string> JEScriptNode::Eyes = {
	{ 0, "serious" },
	{ 1, "happy" },
	{ 2, "decided" },
	{ 3, "special" },
	{ 4, "sad" },
	{ 5, "surprised" },
	{ 6, "neutral_blush" },
	{ 7, "happy_blush" },
	{ 8, "decided_blush" },
	{ 9, "special_blush" },
	{ 10, "sad_blush" },
	{ 11, "surprised_blush" },
};
map<int, string> JEScriptNode::Mouths = {
	{ 10, "sad_blush" },
	{ 12, "neutral" },
	{ 13, "talk" },
	{ 14, "serious" },
	{ 15, "yell" },
	{ 16, "tense" },
	{ 17, "stressed" },
};
map<int, string> JEScriptNode::Names = {
	{ 0, "player" },
	{ 2, "partner" },
	{ 106, "unknown" }, // May alse be Rob ???
	{ 107, "murno" },
	{ 108, "vee" },
	{ 109, "tier" },
	{ 110, "lemmy" },
	{ 111, "jade" },
	{ 112, "velworen" },
	{ 113, "thuss" },
	{ 114, "roche" },
	{ 115, "unknown" }, // Not sure
	{ 116, "unknown" }, // Not sure
	{ 118, "gatekeeper" },
	{ 119, "chief" },
	{ 120, "man" },
	{ 121, "woman" },
	{ 122, "elderly" },
	{ 123, "child" },
	{ 124, "shop_owner" },
	{ 125, "shop_clerk" },
	{ 126, "merchant" },
	{ 127, "reception" },
	{ 128, "unknown" }, // Not sure
	{ 129, "wellman" },
	{ 130, "ianna" },
	{ 131, "zakk" },
	{ 132, "jade" },
	{ 133, "benson" },
	{ 134, "anise" },
	{ 135, "tram" },
	{ 136, "gallahan" },
	{ 137, "serge" },
	{ 138, "elieze" },
	{ 141, "phantom dragon" },
	{ 142, "magdrad" },
	{ 143, "archer_girl" },
	{ 144, "stray_cat" },
	{ 145, "cat lover" },
	{ 146, "fisherman" },
	{ 147, "lumberjack" },
	{ 148, "peddler\'s_mother" },
	{ 149, "peddler\'s_daughter" },
	{ 150, "wanted man" },
	{ 151, "jailer" },
	{ 152, "zenichi" },
	{ 153, "zenji" },
	{ 154, "zenzou" },
	{ 155, "zentatsu" },
};

// Make special cases when text type = location or choice or menu

JEScriptNode::JEScriptNode(ScriptState ss)
{
	tabLevel = 0;
	state = ss;

	if ((!state.portrait_l.active &&
		!state.portrait_r.active)
		&&
		(state.portrait_l.name != -1 ||
		state.portrait_r.name != -1))
	{
		state.info.simplebox = true;
	}

	makeJEText();
}

void JEScriptNode::addFemale(ScriptState ss)
{
	AllText.clear();
	AllText.push_back("<" + state.textType + ">");
	tabLevel++;

	makeInfo(state.info);
	makePortraits(state.portrait_l, ss.portrait_r);

	string tab(tabLevel, '\t');
	tabLevel++;

	AllText.push_back(tab + "<male>");
	makeSjis(state.sjis);
	AllText.push_back(tab + "</male>");

	AllText.push_back(tab + "<female>");
	makeSjis(ss.sjis);
	AllText.push_back(tab + "</female>");
	tabLevel--;

	tabLevel--;
	AllText.push_back("</" + ss.textType + ">");
}

void JEScriptNode::makeOptionNode(ScriptState ss, string headName)
{
	state = ss;

	AllText.push_back("<" + headName +">");
	tabLevel++;

	string tab(tabLevel, '\t');
	if (state.textType == "menutitle")
	{
		AllText.push_back(tab + "<title>");
		tabLevel++;
		makeSjis({ state.sjis.front() });
		state.sjis.pop_front();
		tabLevel--;
		AllText.push_back(tab + "</title>");
	}
	else if (state.textType == "choice")
	{
		// Choice Box is always simple
		AllText.push_back(tab + "<info box=\"simple\">");
	}

	for (auto const& i : state.sjis)
	{
		AllText.push_back(tab + "<option>");
		tabLevel++;
		makeSjis({ i });
		tabLevel--;
		AllText.push_back(tab + "</option>");
	}

	tabLevel--;
	AllText.push_back("</" + headName + ">");
}

void JEScriptNode::makeJEText()
{
	AllText.push_back("<" + state.textType + ">");
	tabLevel++;

	string tab(tabLevel, '\t');

	if (state.textType != "location" &&
		state.textType != "popup")
	{
		if (!state.ungendered())
		{
			if (state.male)
				AllText.push_back(tab + "<player gender=\"male\">");
			else if (state.female)
				AllText.push_back(tab + "<player gender=\"female\">");
		}

		if (!state.unpartnered())
		{
			string partner = "";
			if (state.rundor)
				partner = "run-dor";
			else if (state.enzi)
				partner = "enzi";
			else if (state.killfith)
				partner = "killfith";
			else if (state.rufeel)
				partner = "rufeel";
			AllText.push_back(tab + "<partner name=\"" + partner + "\">");
		}

		makeInfo(state.info);
		makePortraits(state.portrait_l, state.portrait_r);
	}
	
	makeSjis(state.sjis);

	tabLevel--;
	AllText.push_back("</" + state.textType + ">");
}

void JEScriptNode::makeInfo(Info info)
{
	string tab(tabLevel, '\t');

	string infoText = tab + "<info ";
	size_t defaultSize = infoText.size();
	if (info.simplebox)
		infoText = infoText + "box=\"simple\"";
	else if (info.nobox)
		infoText = infoText + "box=\"nobox\"";
	else if (info.left && info.right)
		infoText = infoText + "box=\"both\"";
	else if (info.left)
		infoText = infoText + "box=\"left\"";
	else if (info.right)
		infoText = infoText + "box=\"right\"";

	if (infoText.size() > defaultSize)
		AllText.push_back(infoText + ">");

}

void JEScriptNode::makePortraits(Portrait p_l, Portrait p_r)
{
	string tab(tabLevel, '\t');	
	
	// Making sure all properties are set before displaying line

	string pText = tab + "<portrait_l";
	size_t defaultSize = pText.size();
	if (p_l.active)
	{
		if (p_l.entry != -1)
		{
			if (Characters.contains(p_l.entry))
				pText = pText + " entry=\"" + Characters[p_l.entry] + "\"";
			else
			{
				pText = pText + " entry=\"" + to_string(p_l.entry) + "\"";
				cout << "Unknown entry: " << p_l.entry << "\n";
			}
		}

		if (p_l.eye != -1)
		{
			if (Eyes.contains(p_l.eye))
				pText = pText + " eye=\"" + Eyes[p_l.eye] + "\"";
			else
			{
				pText = pText + " eye=\"" + to_string(p_l.eye) + "\"";
				cout << "Unknown eye: " << p_l.eye << "\n";
			}
		}

		if (p_l.mouth != -1)
		{
			if (Mouths.contains(p_l.mouth))
				pText = pText + " mouth=\"" + Mouths[p_l.mouth] + "\"";
			else
			{
				pText = pText + " mouth=\"" + to_string(p_l.mouth) + "\"";
				cout << "Unknown mouth: " << p_l.mouth << "\n";
			}
		}		
	}
	// This is probably a mistake
	// Need to find a better/different way to determine when to do this
	else if (p_l.name != -1 && state.info.simplebox)
	{
		if (Names.contains(p_l.name))
			pText = pText + " name=\"" + Names[p_l.name] + "\"";
		else
		{
			pText = pText + " name=\"" + to_string(p_l.name) + "\"";
			cout << "Unknown name: " << p_l.name << "\n";
		}
	}

	if (pText.size() > defaultSize)
		AllText.push_back(pText + ">");
	

	pText = tab + "<portrait_r";
	defaultSize = pText.size();
	if (p_r.active)
	{
		if (p_r.entry != -1)
		{
			if (Characters.contains(p_r.entry))
				pText = pText + " entry=\"" + Characters[p_r.entry] + "\"";
			else
			{
				pText = pText + " entry=\"" + to_string(p_r.entry) + "\"";
				cout << "Unknown entry: " << p_r.entry << "\n";
			}
		}

		if (p_r.eye != -1)
		{
			if (Eyes.contains(p_r.eye))
				pText = pText + " eye=\"" + Eyes[p_r.eye] + "\"";
			else
			{
				pText = pText + " eye=\"" + to_string(p_r.eye) + "\"";
				cout << "Unknown eye: " << p_r.eye << "\n";
			}
		}

		if (p_r.mouth != -1)
		{
			if (Mouths.contains(p_r.mouth))
				pText = pText + " mouth=\"" + Mouths[p_r.mouth] + "\"";
			else
			{
				pText = pText + " mouth=\"" + to_string(p_r.mouth) + "\"";
				cout << "Unknown mouth: " << p_r.mouth << "\n";
			}
		}
	}
	// This is probably a mistake
	// Need to find a better/different way to determine when to do this
	else if (p_r.name != -1 && state.info.simplebox)
	{
		if (Names.contains(p_r.name))
			pText = pText + " name=\"" + Names[p_r.name] + "\"";
		else
		{
			pText = pText + " name=\"" + to_string(p_r.name) + "\"";
			cout << "Unknown name: " << p_r.name << "\n";
		}
	}

	if (pText.size() > defaultSize)
		AllText.push_back(pText + ">");
}

void JEScriptNode::makeSjis(list<string> sjis)
{
	string tab(tabLevel, '\t');
	AllText.push_back(tab + "<sjis>");
	tabLevel++;

	tab = string(tabLevel, '\t');
	for (auto const& i : sjis)
	{		
		string line = ST.replaceTags(i);
		AllText.push_back(tab + line + "<end_line>");
	}

	tabLevel--;
	tab = string(tabLevel, '\t');
	AllText.push_back(tab + "</sjis>");
}
