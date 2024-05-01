#include "JEScript.h"


void JEScript::createJEScript()
{
	ofstream stream(filename);

	for (auto const& i : nodes)
	{
		list<string> text = i->GetText();
		for (auto const& j : text)
		{
			stream << j << "\n";
		}
	}

	stream.close();
}
