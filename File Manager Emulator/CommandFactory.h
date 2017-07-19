#pragma once
#include "Command.h"

#include <vector>

template <class COMMAND>
struct tConcreteCommandFactory
{
		static std::unique_ptr<tCommand> create_concrete_command(const std::vector<std::string>& parsedString)
		{
			return std::unique_ptr<tCommand> (new COMMAND (parsedString));
		}
};

struct tCommandFactory
{
	enum class CommandType
	{
		MAKE_DIR, 
		CHANGE_DIR, 
		REMOVE_DIR, 
		RECURSIVE_DEL, 
		MAKE_FILE, 
		MAKE_HARD_LINK, 
		MAKE_SOFT_LINK, 
		DEL, 
		COPY, 
		MOVE
	};
	static std::unique_ptr<tCommand> create_command(const std::string& newCommandText);
};

std::vector<std::string> parse_command_text(const std::string& newCommandText);
tCommandFactory::CommandType command_type_by_name(const std::string& name);
