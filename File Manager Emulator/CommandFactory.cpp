#include "stdafx.h"
#include "CommandFactory.h"
#include "Commands.h"
#include <filesystem>
#include "CIString.h"

std::unique_ptr<tCommand> tCommandFactory :: create_command(tFileManager* receiver, const ci_string& newCommandText)
{
	auto parsedString = parse_command_text(newCommandText);
	if (parsedString.empty()) {
		throw std::runtime_error("cannot parse command");
	}
	auto comByNameIt = TypesByName.find(parsedString[0]);
	if (comByNameIt == TypesByName.end()) {
		throw std::runtime_error("command not supported");
	}
	return TypesByName[parsedString[0]](receiver, parsedString);
}

tCommandFactory::tCommandFactory ()
	: TypesByName{
		{ "MD", tConcreteCommandFactory<tMakeDirCommand>::create_concrete_command },
		{ "CD", tConcreteCommandFactory<tChangeDirCommand>::create_concrete_command },
		{ "RD", tConcreteCommandFactory<tRemoveDirCommand>::create_concrete_command },
		{ "DELTREE", tConcreteCommandFactory<tRecursiveRemoveDirCommand>::create_concrete_command },
		{ "MF", tConcreteCommandFactory<tMakeFileCommand>::create_concrete_command },
		{ "MHL", tConcreteCommandFactory<tMakeHardLinkCommand>::create_concrete_command },
		{ "MDL", tConcreteCommandFactory<tMakeSoftLinkCommand>::create_concrete_command },
		{ "DEL", tConcreteCommandFactory<tDelCommand>::create_concrete_command },
		{ "COPY", tConcreteCommandFactory<tCopyCommand>::create_concrete_command },
		{ "MOVE", tConcreteCommandFactory<tMoveCommand>::create_concrete_command },
	}
{
}

std::vector<ci_string> parse_command_text (const ci_string& newCommandText)
{
	const auto ws = ' ';
	std::vector<ci_string> splits;
	size_t begin = 0, ws_pos;
	while (begin < newCommandText.length() && newCommandText[begin] == ws)
		++begin;
	do {
		ws_pos = newCommandText.find(ws, begin);
		splits.push_back(newCommandText.substr(begin, ws_pos - begin));
		begin = ws_pos + 1;
		
	} while (ws_pos != ci_string::npos);
	return splits;
}
