#pragma once
#include "Command.h"

#include <vector>
#include <unordered_map>
#include <functional>

template <class COMMAND>
struct tConcreteCommandFactory
{
	static std::unique_ptr<tCommand> create_concrete_command(tFileManager* receiver, std::vector<ci_string>& parsedString)
	{
		return std::unique_ptr<tCommand>(new COMMAND(receiver, parsedString ));
	}
};

//singleton
class tCommandFactory
{
public:
	tCommandFactory();
	tCommandFactory(tCommandFactory&) = delete;
	tCommandFactory(tCommandFactory&&) = delete;
	tCommandFactory& operator=(tCommandFactory&) = delete;
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

	std::unique_ptr<tCommand> create_command(tFileManager* receiver, const ci_string& newCommandText);

private:
	using funcPtr = std::function<std::unique_ptr<tCommand>(tFileManager*, std::vector<ci_string>&)>;
	std::unordered_map<ci_string, funcPtr, decltype(&ci_string_hash)> TypesByName;
};

std::vector<ci_string> parse_command_text(const ci_string& newCommandText);
