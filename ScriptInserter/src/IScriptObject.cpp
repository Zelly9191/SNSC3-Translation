#include "IScriptObject.h"


IScriptObject::IScriptObject(int lineNumber, list<string> sjisText)
{
	IScriptObject::lineNumber = lineNumber;
	IScriptObject::sjisText = sjisText;
}
