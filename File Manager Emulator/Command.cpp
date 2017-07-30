#include "stdafx.h"
#include "Command.h"

#include <string>
#include "CIString.h"

tCommand::tCommand(tFileManager* receiver, std::vector<ci_string>& parsedStr) :
	Receiver{ receiver },
	Name{ parsedStr[0] }
{
	split_path(&Source, parsedStr[1]);
	if (parsedStr.size() > 2) {
		 split_path(&Dest, parsedStr[2]);
	}
}

void tCommand :: split_path(tPath* parsedPath, const ci_string& text)
{
	const auto slash = '\\';
	size_t begin = 0, slash_pos;
	auto firstSqBracket = text.find('[');
	if (firstSqBracket != ci_string::npos) {
		begin = firstSqBracket + 1;
		auto linkText = text.substr(0, firstSqBracket);
		if (linkText == "hlink") {
			parsedPath->IsHardLink = true;
		} else if (linkText == "dlink") {
			parsedPath->IsSoftLink = true;
		}
	}
	do {
		slash_pos = text.find(slash, begin);
		parsedPath->Path.push_back(text.substr(begin, slash_pos - begin));
		begin = slash_pos + 1;
	} while (slash_pos != ci_string::npos);
	if (firstSqBracket != ci_string::npos) {
		auto name = parsedPath->Path.back();
		name = name.substr(0, name.rfind(']'));
	}
}