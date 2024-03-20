#include <string>
using namespace std;

void findXML(string fileName, string fileIdentifier, string *report);
void gatherData(string txtFile, string xmlFile, string *report);
bool validateXML(string xmlFile, string *report);
void checkLines(ifstream *xmlStream, string currentLine, string tagType, int *lineCount, bool *noErrors, string *report, bool *locationTag);
