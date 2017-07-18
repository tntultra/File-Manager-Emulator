#pragma once

#include "Command.h"

class tMoveCommand : public tCommand
{
	struct tMoveCommandImpl;
	std::unique_ptr<tMoveCommandImpl> pImpl;

	virtual void executeImpl() const override;
};

