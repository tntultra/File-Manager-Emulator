#include "stdafx.h"

#include "Commands.h"

struct tCommandImpl
{
	explicit tCommandImpl(const std::vector<std::string>& parsedStr) :
		Source{ parsedStr[1] },//this can throw, because this is unique situation
		Dest{ parsedStr.size() > 2 ? parsedStr[2] : std::string{} }//this requires to be optional
	{
	}

	std::string Source;
	std::string Dest;//should be optional
};