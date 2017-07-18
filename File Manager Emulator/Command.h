#pragma once
#include <string>
#include <memory>

//abstract class, interface
class tCommand
{
	virtual void executeImpl() const = 0;
	//possible undo etc
	//virtual void undoImpl () const = 0;
	
public:
	virtual ~tCommand() = default;

	//you dont want to move command
	
	//possible clone
	//std::unique_ptr<tCommand> clone () const;
	void execute() const
	{
		return executeImpl();
	}
};