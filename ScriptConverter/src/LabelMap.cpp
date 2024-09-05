#include "LabelMap.h"
#include "CodeParser.h"

LabelMap::LabelMap() {}

LabelMap::LabelMap(list<string> text)
{
	AllText = text;
}

void LabelMap::analyzeText()
{
	Labels = map<string, IScriptNode>();
	IScriptNode node;
	node.genderText = false;
	node.partnerText = false;

	if (!AllText.front().starts_with("@start"))
		node.label = "@start";

	// Menu code in IScript is weird, so i have to treat it different.
	bool inMenu = false;

	int lineNumber = 0;
	for (auto const& i : AllText)
	{
		lineNumber++;
		// Label has ended
		if (i == "" || i.empty())
		{
			Labels[node.label] = node;
		}
		else if (i.starts_with("@"))
		{
			// Edge case of new label starting without a blank line before it
			if (!node.label.empty())
			{
				//cout << node.label << "\n";
				node.next = i.substr(0, i.size() - 1);
				Labels[node.label] = node;
			}

			node = IScriptNode();
			node.genderText = false;
			node.partnerText = false;
			node.offset = lineNumber;
			// Remove ":" at the end of label declaration
			string label = i.substr(0, i.size() - 1);
			node.label = label;
			Labels[label] = node;
			continue;
		}
		// determine next node
		else if (i.starts_with("goto"))
		{
			if (i.starts_with("gotomap"))
				continue;
			string next = i.substr(4);
			size_t pos = next.find_first_not_of(" \t");
			if (pos != string::npos)
				next = next.substr(pos);

			node.next = next;
			Labels[node.label] = node;
		}
		// End of @Func or @Cond
		else if (i.starts_with("ret"))
		{
			Labels[node.label] = node;
		}
		else
		{
			// Some nodes have no label.
			// Typically means the game doesnt read that code (i assume)
			// But sometimes they contain text.
			if (node.label.empty())
			{
				node = IScriptNode();
				node.genderText = false;
				node.partnerText = false;
				node.offset = lineNumber;
				string label = "@Line_" + to_string(lineNumber);
				node.label = label;
				Labels[label] = node;
			}

			size_t pos = i.find_first_of(" \t");
			string code = "";
			if (pos != string::npos)
				code = i.substr(0, pos);
			else
				code = i;

			if (!CodeParser::ValidCodes.contains(code))
			{
				if (inMenu)
				{
					node.lines.push_back("");
					continue;
				}

				if (i.find("@") != string::npos)
				{
					//cout << "Code: " << code << "\n";
					CodeParser::ValidCodes[code] = 'f';
				}
				else
				{
					node.lines.push_back("");
					continue;
				}
			}

			if (inMenu && code == "jumpz")
			{
				node.lines.push_back("");
				continue;
			}

			if (code == "menutxt" ||
				code == "menutitle")
				inMenu = true;
			else if (code == "menuwait")
				inMenu = false;

			// Has gender dependent text
			if (i.find("$b002 ==") != string::npos)
			{
				node.genderText = true;
				//node.lines.push_back(i);

				HasGenderText = true;
			}
			// Has partner dependent text
			else if (i.find("$b003 ==") != string::npos)
			{
				node.partnerText = true;
				//node.lines.push_back(i);

				HasPartnerText = true;
			}
			node.lines.push_back(i);
			continue;
		}

		node = IScriptNode();
	}
}

list<string> LabelMap::Keys()
{
	list<string> keys;

	for (const auto& i : Labels) {
		keys.push_back(i.first);
	}

	return keys;
}
