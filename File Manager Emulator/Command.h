#pragma once
#include <string>
#include <memory>
#include <vector>

//abstract class, interface
class tCommand
{
	//virtual std::unique_ptr<tCommand> createImpl(const std::vector<std::string>& parsedString) const = 0;
	virtual void executeImpl() const = 0;

public:
	virtual ~tCommand() = default;

	void execute() const
	{
		return executeImpl();
	}

};