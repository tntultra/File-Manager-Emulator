#include "stdafx.h"

#include "Commands.h"
#include "FileManager.h"

void tMoveCommand::executeImpl () const
{
	Receiver->move(Source, Dest);
}

void tCopyCommand::executeImpl () const
{
	Receiver->copy(Source, Dest);
}
void tDelCommand::executeImpl () const
{
	Receiver->delete_file_or_link(Source);
}
void tMakeDirCommand::executeImpl () const
{
	Receiver->create_dir(Source);
}
void tChangeDirCommand::executeImpl () const
{
	Receiver->change_dir(Source);
}
void tRemoveDirCommand::executeImpl () const
{
	Receiver->remove_dir(Source);
}
void tRecursiveRemoveDirCommand::executeImpl () const
{
	Receiver->recursive_remove_dir(Source);
}
void tMakeFileCommand::executeImpl () const
{
	Receiver->create_file(Source);
}
void tMakeSoftLinkCommand::executeImpl () const
{
	Receiver->create_soft_link(Source, Dest);
}

void tMakeHardLinkCommand::executeImpl () const
{
	Receiver->create_hard_link(Source, Dest);
}
