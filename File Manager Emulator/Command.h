#pragma once
#include <memory>
#include <string>
#include <vector>
#include "FileManager.h"

struct tCommandImpl;

//abstract class, interface
class tCommand
{
	virtual void executeImpl() const = 0;
protected:
	tFileManager* Receiver;
	std::string Name;
	std::string Source;
	std::string Dest;//should be optional
public:
	tCommand(const tFileManager* receiver, const std::vector<std::string>& parsedStr);
	virtual ~tCommand() = default;

	void execute() const
	{
		return executeImpl();
	}

};