#pragma once
#include <memory>
#include <string>
#include <vector>

struct tCommandImpl;

//abstract class, interface
class tCommand
{
	//virtual std::unique_ptr<tCommand> createImpl(const std::vector<std::string>& parsedString) const = 0;
	virtual void executeImpl() const = 0;
protected:
	uptr<tCommandImpl> pImpl;
public:
	//explicit tCommand(const std::vector<std::string>& parsedStr);//throws out of range exception
	virtual ~tCommand() = default;

	void execute() const
	{
		return executeImpl();
	}

};