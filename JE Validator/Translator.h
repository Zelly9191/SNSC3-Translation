#include <string>
using namespace std;

void findXML(string fileName, string fileIdentifier);
void gatherData(string txtFile, string xmlFile);
bool validateXML(string xmlFile);
void checkLines(ifstream *xmlStream, string currentLine, string tagType, int *lineCount, bool *noErrors);
