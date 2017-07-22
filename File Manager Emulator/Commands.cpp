#include "stdafx.h"

#include "Commands.h"

tMoveCommand::tMoveCommand (std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString)
	: tCommand{ receiver, parsedString}
{}

void tMoveCommand::executeImpl () const
{
	if (Receiver.expired ()) {
		Receiver.lock()->move(Source, Dest);
	}
}
