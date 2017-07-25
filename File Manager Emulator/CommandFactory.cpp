#include "stdafx.h"
#include "CommandFactory.h"
#include "Commands.h"
#include <filesystem>

std::unique_ptr<tCommand> tCommandFactory :: create_command(tFileManager* receiver, const std::string& newCommandText)
{
	auto parsedString = parse_command_text(newCommandText);
	if (parsedString.empty()) {
		throw std::exception("cannot parse command");
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

std::vector<std::string> parse_command_text (const std::string& newCommandText)
{
	const char ws = ' ';
	std::vector<std::string> splits;
	size_t begin = 0, ws_pos;
	do {
		while (begin < newCommandText.length () && newCommandText[begin] == ws) 
			++begin;
		ws_pos = newCommandText.find(ws);
		splits.push_back(newCommandText.substr(begin, ws_pos - begin));
		begin = ws_pos + 1;
		
	} while (ws_pos != std::string::npos);
	return splits;
}
