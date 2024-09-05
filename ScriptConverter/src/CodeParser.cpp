#include "CodeParser.h"
#include "JEScriptNode.h"

map<string, char> CodeParser::ValidCodes = {
	{ "dialogtxt", 'd'},
	{ "dialogbig", 'b' },
	{ "placetxt", 'l'},
	{ "popuptxt", 'u'},
	{ "choicetxt", 'c'},
	{ "choicewait", 'w'},
	{ "menutxt", 'm'},
	{ "menutitle", 'n'},
	{ "menuwait", 'o'},
	{ "jumpz", 'j'},
	{ "call", 'g'},
	{ "mapev", 'f'},
	{ "code0304", 'h'},
	{ "code0309", 'w'},
	{ "code030a", 'w'},
	{ "code030c", 'w'},
	{ "code0478", 's'},
	{ "code0479", 'p'},
	{ "code047a", 'a'},
	{ "code047d", 'x'},
	{ "code047e", 'i'},
};

CodeParser::CodeParser(LabelMap labMap)
{
	CodeParser::labMap = labMap;
	choiceLabels = {};
}

void CodeParser::parseCode(ScriptState ss, string label)
{
	state = ss;

	if (!label.starts_with("@Func"))
		UnexploredNodes.remove(label);

	IScriptNode node = labMap[label];
	state.label = label;
	//int lineNum = -1;
	int lineNum = node.offset;
	string next = node.next;
	//cout << label << ":\t" << ss.nodeID << "\n";
	for (auto const& i : node.lines)
	{
		lineNum++;
		if (i.empty())
			continue;
		if (!parseLine(i, lineNum))
		{
			next = "";
			break;
		}
	}

	// Node is finished at this point


	while (!choiceLabels.empty())
	{
		string cLabel = choiceLabels.front();
		choiceLabels.pop_front();
		go(cLabel);
	}

	while (!menuLabels.empty())
	{
		string mLabel = menuLabels.front();
		menuLabels.pop_front();
		go(mLabel);
	}

	//cout << "Finished " << label << "\n\n";
	if (!next.empty())
	{
		go(next);
	}


	
	
}

bool CodeParser::parseLine(string line, int lineNum)
{
	// Parse code

		// can switch on char

		// separate code and arguments
	string code;
	string args;
	// search for space after end of code
	size_t pos = line.find_first_of(" \t");
	// there is space after the code. may have args.
	if (pos != string::npos)
	{
		code = line.substr(0, pos);

		string possibleArgs = line.substr(pos);
		size_t pos2 = possibleArgs.find_first_not_of(" \t");
		// has args
		if (pos2 != string::npos)
		{
			args = possibleArgs.substr(pos2);
		}
		// no args
		else
		{
			args = "";
		}
	}
	// no space after code, meaning no args
	else
	{
		code = line;
		args = "";
	}

	// lambda for shared behavior
	auto handleText = [&]() 
		{
			string text = getText(args);
			state.sjis.push_back(text);
			if (state.lineNumber == -1)
				state.lineNumber = lineNum;
		};

	// I prefer a switch statement over endless if else statements
	// Switch can only be done with int or char, so i chose char which is a single letter
	// handle code	
	const char codeHandle = ValidCodes[code];
	switch (codeHandle)
	{
		// Dialogue
	case 'd':
		state.textType = "dialogue";
		handleText();
		break;
		// Big Text
	case 'b':
		state.textType = "bigtext";
		handleText();
		break;
		// Location
	case 'l':
		state.textType = "location";
		handleText();
		clearText();
		break;
		// Popup text
	case 'u':
		state.textType = "popup";
		handleText();
		clearText();
		break;
		// Choice Text
	case 'c':
		state.textType = "choice";
		choiceLabels.push_back(findLabel(args));
		handleText();
		break;
		// Menu text
	case 'm':
		if (state.textType != "menutitle")
			state.textType = "menu";
		menuLabels.push_back(findLabel(args));
		handleText();
		break;
		// Menu title
	case 'n':
		state.textType = "menutitle";
		handleText();
		break;
	case 'o':
		clearText();
		break;
		// Jump
	case 'j':
		return !jumpz(args);
		break;
		// Func call
	case 'g':
		go(args);
		break;
		// Has label in the args
	case 'f':
		go(findLabel(args));
		break;
		// Has label in the args
	case 'h':
		state.info.simplebox = stoi(args);
		break;
		// Wipe/Clear text code
	case 'w':
		clearText();
		break;
	case 's':
		setPortrait(args);
		break;
	case 'p':
		changePortrait(args);
		break;
	case 'a':
		togglePortrait(args);
		break;
	case 'i':
		setBox(args);
		break;
	case 'x':
		setName(args);
		break;
	}

	return true;
}

bool CodeParser::jumpz(string args)
{
	size_t pos = args.find(",");
	string label = args.substr(0, pos);
	string cond = args.substr(pos + 1);

	if (cond.find("$b002") != string::npos)
	{
		int gender = cond[cond.size() - 2] - '0';
		if ((gender == 0 && !state.male) ||
			(gender == 1 && !state.female))
		{
			go(label);
			return true;
		}
	}
	else if (cond.find("$b003") != string::npos)
	{
		int partner = cond[cond.size() - 2] - '0';
		if ((partner == 0 && !state.rundor) ||
			(partner == 1 && !state.enzi) ||
			(partner == 2 && !state.killfith) ||
			(partner == 3 && !state.rufeel))
		{
			go(label);
			return true;
		}
	}
	else
	{
		// Find label and go.
		// Treats jump like a function call,
		// since the condition is not about player gender or partner.
		go(label);
	}

	return false;
}

string CodeParser::findLabel(string args)
{
	size_t pos = args.find("@");
	string newArgs = args.substr(pos);

	// Find first comma. If not one, then there is no parameter after label.
	size_t pos2 = newArgs.find_first_of(",");

	if (pos2 != string::npos)
		newArgs = newArgs.substr(0, pos2);

	return newArgs;
}

// goto @Label. Only ever has 1 arg.
void CodeParser::go(string args)
{
	if (args == "@start")
		return;

	bool found = (std::find(UnexploredNodes.begin(), UnexploredNodes.end(), args) != UnexploredNodes.end());
	// Node has been done before
	if (!found)
		return;

	parseCode(state, args);
}

string CodeParser::getText(string text)
{
	size_t quote1 = text.find("\"");
	if (quote1 == string::npos)
		return "";
	// position of second quote
	size_t quote2 = text.find("\"", quote1 + 1);

	// text in between quotes
	string dialog = text.substr(quote1 + 1, quote2 - quote1 - 1);
	return dialog;
}

void CodeParser::setPortrait(string args)
{
	size_t comma = args.find(",");

	int arg1 = stoi(args.substr(0, comma));
	int arg2 = stoi(args.substr(comma + 2));

	if (arg1 == 0)
		state.portrait_l.entry = arg2;
	else if (arg1 == 1)
		state.portrait_r.entry = arg2;
}

void CodeParser::togglePortrait(string args)
{
	size_t comma = args.find(",");
	int arg1 = stoi(args.substr(0, comma));

	size_t comma2 = args.find(",", comma + 1);
	bool arg2 = stoi(args.substr(comma + 2, comma2 - comma - 1));

	// Not used far as I know
	//int arg3 = stoi(args.substr(comma2 + 2));

	if (arg1 == 0)
	{
		state.portrait_l.active = arg2;
	}
	else if (arg1 == 1)
	{
		state.portrait_r.active = arg2;
	}
}

void CodeParser::changePortrait(string args)
{
	size_t comma = args.find(",");
	int arg1 = stoi(args.substr(0, comma));

	size_t comma2 = args.find(",", comma + 1);
	int arg2 = stoi(args.substr(comma + 2, comma2 - comma - 1));

	int arg3 = stoi(args.substr(comma2 + 2));

	if (arg1 == 0)
	{
		state.portrait_l.eye = arg2;
		state.portrait_l.mouth = arg3;
	}
	else if (arg1 == 1)
	{
		state.portrait_r.eye = arg2;
		state.portrait_r.mouth = arg3;
	}
}

void CodeParser::setName(string args)
{
	size_t comma = args.find(",");

	int arg1 = stoi(args.substr(0, comma));
	// 0 = false, any nonzero = true. This is c++ default behavior.
	int arg2 = stoi(args.substr(comma + 2));

	if (arg1 == 0)
		state.portrait_l.name = arg2;
	else if (arg1 == 1)
		state.portrait_r.name = arg2;
}

void CodeParser::setBox(string args)
{
	size_t comma = args.find(",");

	int arg1 = stoi(args.substr(0, comma));
	// 0 = false, any nonzero = true. This is c++ default behavior.
	bool arg2 = stoi(args.substr(comma + 2));

	if (arg1 == 0)
		state.info.left = arg2;
	else if (arg1 == 1)
		state.info.right = arg2;
}

void CodeParser::clearText()
{
	SavedStates.push_back(state);
	state.textType = "";
	state.sjis.clear();
	state.lineNumber = -1;
}
