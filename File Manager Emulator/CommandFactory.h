#pragma once
#include <memory>
#include "Command.h"

class tCommandFactory
{
	public:
		std::unique_ptr<tCommand> parse_command(const std::string& newCommandText);
};
