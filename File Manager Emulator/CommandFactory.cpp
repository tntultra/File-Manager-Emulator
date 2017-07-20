#include "stdafx.h"
#include "CommandFactory.h"
#include "Commands.h"

std::unique_ptr<tCommand> tCommandFactory :: create_command(const std::string& newCommandText)
{
	auto parsedString = parse_command_text(newCommandText);
	if (parsedString.empty()) {
		throw std::exception("cannot parse command");
	}
	switch (command_type_by_name (parsedString[0])) {
	case CommandType::MAKE_DIR:
		return tConcreteCommandFactory<tMakeDirCommand>::create_concrete_command(parsedString);
	case CommandType::CHANGE_DIR:
		return tConcreteCommandFactory<tChangeDirCommand>::create_concrete_command(parsedString);
	case CommandType::REMOVE_DIR:
		return tConcreteCommandFactory<tRemoveDirCommand>::create_concrete_command(parsedString);
	case CommandType::RECURSIVE_DEL:
		return tConcreteCommandFactory<tRecursiveRemoveDirCommand>::create_concrete_command(parsedString);
	case CommandType::MAKE_FILE:
		return tConcreteCommandFactory<tMakeFileCommand>::create_concrete_command(parsedString);
	case CommandType::MAKE_HARD_LINK:
		return tConcreteCommandFactory<tMakeHardLinkCommand>::create_concrete_command(parsedString);
	case CommandType::MAKE_SOFT_LINK:
		return tConcreteCommandFactory<tMakeSoftLinkCommand>::create_concrete_command(parsedString);
	case CommandType::DEL:
		return tConcreteCommandFactory<tDelCommand>::create_concrete_command(parsedString);
	case CommandType::COPY:
		return tConcreteCommandFactory<tCopyCommand>::create_concrete_command(parsedString);
	case CommandType::MOVE:
		return tConcreteCommandFactory<tMoveCommand>::create_concrete_command(parsedString);
	default:
		throw std::exception("bad command name");
	}
}
