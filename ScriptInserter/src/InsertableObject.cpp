#include "InsertableObject.h"


InsertableObject::InsertableObject()
{
	lineNumber = 0;
	sjisText = {};
	asciiText = {};
	offset = 0;
	used = false;
}

void InsertableObject::makeInsertableObject(JEObject jeObj, IScriptObject iScriptObj)
{
	lineNumber = iScriptObj.lineNumber;
	sjisText = iScriptObj.sjisText;
	asciiText = jeObj.asciiText;

	int iSize = sjisText.size();
	int jeSize = asciiText.size();
	offset = jeSize - iSize;

	used = false;
}
