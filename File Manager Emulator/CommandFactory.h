#pragma once
#include "Command.h"

#include <vector>
#include <map>
#include <functional>


template <class COMMAND>
struct tConcreteCommandFactory
{
	static std::unique_ptr<tCommand> create_concrete_command(tFileManager* receiver, const std::vector<std::string>& parsedString)
	{
		return std::unique_ptr<tCommand>(new COMMAND(receiver, parsedString ));
	}
};

//singleton
class tCommandFactory
{
public:
	tCommandFactory();
	tCommandFactory(const tCommandFactory&) = delete;
	tCommandFactory(tCommandFactory&&) = delete;
	tCommandFactory& operator=(const tCommandFactory&) = delete;
	tCommandFactory& operator=(tCommandFactory&&) = delete;
	~tCommandFactory() = default;

	/*enum class CommandType
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
	};*/

	std::unique_ptr<tCommand> create_command(tFileManager* receiver, const std::string& newCommandText);

private:
	using funcPtr = std::function<std::unique_ptr<tCommand>(tFileManager*, const std::vector<std::string>&)>;
	std::map<std::string, funcPtr> TypesByName;
};

std::vector<std::string> parse_command_text(const std::string& newCommandText);
