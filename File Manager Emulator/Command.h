#pragma once
#include <memory>
#include <string>
#include <vector>

struct tCommandImpl;

//abstract class, interface
class tCommand
{
	virtual void executeImpl() const = 0;
protected:
	std::unique_ptr<tCommandImpl> pImpl;
public:
	virtual ~tCommand() = default;

	void execute() const
	{
		return executeImpl();
	}

};