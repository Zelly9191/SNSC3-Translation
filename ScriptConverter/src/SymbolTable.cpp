#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	loadTAGS();
}

string SymbolTable::replaceTags(string line)
{
	string newLine = line;
	for (auto const& i : TAGS)
	{
		string tag = i.first;

		size_t pos = newLine.find(tag);
		// Loop handles having multiple of the same tag on a single line
		while (pos != string::npos)
		{
			size_t tagSize = tag.size();

			newLine = newLine.substr(0, pos) + i.second + newLine.substr(pos + tagSize);

			pos = newLine.find(tag);

			//cout << newLine << "\t" << i.second << "\n";
		}
	}

	return newLine;
}

void SymbolTable::loadTAGS()
{
	std::filesystem::path tagPath = "Data/TAGS.csv";
	ifstream stream(tagPath);

	string text;

	while (getline(stream, text))
	{
		// Handles first line in csv with header
		if (text.find("JEScript") != string::npos)
			continue;

		// Find first comma and split text on it
		size_t pos = text.find(",");
		string jeTag = text.substr(0, pos);
		string iTag = text.substr(pos + 1);
		if (iTag.empty())
			iTag = "";
		TAGS.insert({ iTag, jeTag });
	}

	stream.close();
}
