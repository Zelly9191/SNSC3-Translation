#include "IScriptReader.h"
#include "JEScriptNode.h"

IScriptReader::IScriptReader(filesystem::path filename)
{
	IScriptReader::filename = filename.string();
}

void IScriptReader::loadIScript()
{
	ifstream stream(filename);
	string text;

	while (getline(stream, text))
	{
		AllText.push_back(text);
	}

	stream.close();
}

void IScriptReader::createLabelMap()
{
	LabMap = LabelMap(AllText);
	LabMap.analyzeText();
}

void IScriptReader::readIScript()
{
	CodeParser codeParser(LabMap);
	list<ScriptState> states;

	// Determines how many times the script needs to be run
	if (LabMap.HasGenderText && LabMap.HasPartnerText)
	{
		states = {
			ScriptState(0, 0),
			ScriptState(0, 1),
			ScriptState(0, 2),
			ScriptState(0, 3),
			ScriptState(1, 0),
			ScriptState(1, 1),
			ScriptState(1, 2),
			ScriptState(1, 3),
		};
	}
	else if (LabMap.HasGenderText)
	{
		states = {
			ScriptState(0, 4),
			ScriptState(1, 4)
		};
	}
	else if (LabMap.HasPartnerText)
	{
		states = {
			ScriptState(2, 0),
			ScriptState(2, 1),
			ScriptState(2, 2),
			ScriptState(2, 3)
		};
	}
	else
	{
		states = {
			ScriptState(2, 4)
		};
	}
	
	
	int startSize = 0;
	for (auto &i : states)
	{
		codeParser.UnexploredNodes = LabMap.Keys();
		codeParser.parseCode(i, "@start");

		// Unexplored nodes may remain.
		// May sometimes contain extra text.
		list<string> remainingNodes = codeParser.UnexploredNodes;
		while (!remainingNodes.empty())
		{
			string label = remainingNodes.front();
			remainingNodes.pop_front();
			// @Line is a custom label start I made to determine extra nodes.
			if (label.starts_with("@Line"))
			{
				i.Reset();
				codeParser.parseCode(i, label);
			}
		}
		//cout << codeParser.SavedStates.size() << "\n";
		//cout << "Finished" << "\n\n";
		
		//States.insert(States.end(), codeParser.SavedStates.begin(), codeParser.SavedStates.end());
		insertStates(codeParser.SavedStates);
		startSize += int(codeParser.SavedStates.size());
		codeParser.SavedStates.clear();
	}

	for (auto const& j : ScriptMap)
		States.push_back(j.second);

	//cout << "Start: " << startSize << "\tEnd: " << States.size() << "\n";
		

}

void IScriptReader::makeJEScript(std::filesystem::path outfile)
{
	list<JEScriptNode> jeList;

	bool maleFound = false;
	for (auto& i : States)
	{
		if (!i.ungendered())
		{
			// Encountered male Dialogue
			if (i.male && !maleFound)
				maleFound = true;
			// Consecutive Male Dialogue. Should not be merged.
			else if (i.male && maleFound)
				maleFound = false;
			else if (i.female && maleFound)
			{
				// Combine Male/Female Dialogue
				JEScriptNode prevNode = jeList.back();
				jeList.pop_back();
				prevNode.addFemale(i);
				jeList.push_back(prevNode);
				maleFound = false;
				continue;
			}
			else
				maleFound = false;
		}

		JEScriptNode jesn;
		if (i.textType.starts_with("menu"))
			jesn.makeOptionNode(i, "menu");
		else if (i.textType == "choice")
			jesn.makeOptionNode(i, "choice");
		else
			jesn = JEScriptNode(i);
		jeList.push_back(jesn);
	}

	ofstream stream(outfile);
	for (auto const& i : jeList)
	{
		for (auto const& j : i.AllText)
			stream << j << "\n";
	}
	stream.close();
}

void IScriptReader::replaceFunctions()
{
	AllText = FunctionReplacer::replaceFunctions(AllText);
}

void IScriptReader::insertStates(list<ScriptState> states)
{
	// Conveniently, putting all the script states in a map
	// automatically sorts it by its key, the line number
	// dialog starts on. Technically, the keys are ordered in
	// no specific fashion, but its by the int value like 99.9% of the time.

	for (auto const& i : states)
	{
		//cout << i.label << "\n";
		if (!ScriptMap.contains(i.lineNumber))
			ScriptMap[i.lineNumber] = i;
		else
		{
			ScriptState prev = ScriptMap[i.lineNumber];
			
			if (i.male)
				prev.male = true;

			if (i.female)
				prev.female = true;


			if (i.rundor)
				prev.rundor = true;

			if (i.enzi)
				prev.enzi = true;

			if (i.killfith)
				prev.killfith = true;

			if (i.rufeel)
				prev.rufeel = true;

			ScriptMap[i.lineNumber] = prev;
		}
	}
}
