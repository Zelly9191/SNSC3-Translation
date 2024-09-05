#include "globals.h"
#include "IScriptReader.h"
#include "SymbolTable.h"
using namespace std;


SymbolTable ST;

int main(int argc, char* argv[])
{
	ST = SymbolTable();

	string infile = argv[1];
	IScriptReader isr(infile);
	isr.loadIScript();
	isr.createLabelMap();
	isr.readIScript();

	filesystem::path fullpath = infile;
	filesystem::path parentpath = fullpath.parent_path();
	if (!filesystem::is_directory(parentpath / "Done"))
		filesystem::create_directory(parentpath / "Done");

	//filesystem::path outfile = parentpath / "Done" / fullpath.filename().replace_extension(".xml");
	filesystem::path outfile = "Done" / fullpath.filename().replace_extension(".xml");
	
	isr.makeJEScript(outfile);
	//cout << outfile << "\n";
	cout << "Finished " << fullpath.filename() << "\n\n";
	system("pause");

	return 0;
}

