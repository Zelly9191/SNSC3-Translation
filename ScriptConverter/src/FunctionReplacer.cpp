#include "FunctionReplacer.h"

list<string> FunctionReplacer::replaceFunctions(list<string> text)
{
	list<string> t;

	map<string, list<string>> functions = analyzeFunctions(text);

	for (auto const& i : text)
	{
		if (!i.starts_with("call"))
		{
			t.push_back(i);
		}
		else
		{
			size_t pos = i.find("@");
			string fun = i.substr(pos);
			list<string> r = functions.at(fun);
			t.insert(t.end(), r.begin(), r.end());
		}
	}

	return t;
}

map<string, list<string>> FunctionReplacer::analyzeFunctions(list<string> text)
{
	map<string, list<string>> m;
	bool readingFunc = false;

	string key;
	list<string> values;

	for (auto const& i : text)
	{
		if (!i.starts_with("@Func_") && !readingFunc)
		{
			continue;
		}

		// remove colon for key
		if (!readingFunc)
		{
			key = i.substr(0, i.size() - 1);
			readingFunc = true;
		}
		else
		{
			// function over
			if (i.find("ret") != string::npos)
			{
				m.insert({key, values});
				key = "";
				values.clear();
				readingFunc = false;
			}
			else
			{
				string copy = i;
				values.push_back(copy);
			}
		}
	}

	return m;
}
