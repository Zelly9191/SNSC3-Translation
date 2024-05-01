#include "globals.h"
#include "IScriptReader.h"
using namespace std;


int main(int argc, char* argv[])
{
	IScriptReader isr(argv[1]);
	isr.readIScript();
	isr.jeScript.createJEScript();

	system("pause");

	return 0;
}

