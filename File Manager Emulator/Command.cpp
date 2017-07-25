#include "stdafx.h"
#include "Command.h"

#include <string>

tCommand :: tCommand (const tFileManager* receiver, const std::vector<std::string>& parsedStr) :
	Receiver {receiver},
	Name {parsedStr[0]},
	Source{ parsedStr[1] },//this can throw, because this is unique situation
	Dest{ parsedStr.size() > 2 ? parsedStr[2] : std::string{} }//this requires to be optional
{}
