#include "globals.h"
#include "JEScriptReader.h"
#include "ScriptInserter.h"
#include "SymbolTable.h"
using namespace std;

void makeDirectory();
string findMatchingFile(string filename);

SymbolTable ST;

int main()
{
    // Create directory for translated I Scripts
    makeDirectory();

    // Generate a table to replace tags like <three_dots>
    ST = SymbolTable();

    // Iterate through every file in a directory for JE Scripts
    for (auto const& i : filesystem::recursive_directory_iterator(JEScriptDir))
    {
        filesystem::path filename = i.path().filename();

        // Get matching I script
        string iscriptName = findMatchingFile(filename.stem().string());

        cout << " Starting inserting process for " << filename.string() <<
            " into " << iscriptName << "\n";

        // If there is no matching I script
        if (!filesystem::exists(iscriptDir + iscriptName))
        {
            cerr << "no IScript with the name: " << iscriptName << "\n\n";
            continue;
        }

        // Read JE script and extract text
        cout << "Reading JE Script" << "\n";
        JEScriptReader jsr(i.path().string());
        jsr.readFile();
        jsr.formatText();

        // Read untranslated I script
        cout << "Reading I Script" << "\n";
        ScriptInserter inserter(iscriptDir + iscriptName, jsr.JEObjects);
        inserter.readFile();
        cout << "Inserting " << filename.string() << " into " << iscriptName << "\n\n";
        // Write translated I script to a new file
        inserter.replaceFile(insertedDir + iscriptName);
    }

    system("pause");

    return 0;
}

void makeDirectory()
{
    if (!filesystem::is_directory(insertedDir) || !filesystem::exists(insertedDir))
    {
        filesystem::create_directory(insertedDir);
    }

    if (filesystem::exists(nomatchFile))
    {
        filesystem::remove(nomatchFile);
    }
}

string findMatchingFile(string filename)
{
    size_t pos = filename.rfind("_");
    return filename.substr(pos + 1) + ".txt";
}