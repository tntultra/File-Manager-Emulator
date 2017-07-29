#include "stdafx.h"

#include "Commands.h"
#include "FileManager.h"

void tMoveCommand::executeImpl ()
{
	Receiver->move(Source.Path, Dest.Path);
}

void tCopyCommand::executeImpl ()
{
	Receiver->copy(Source.Path, Dest.Path);
}
void tDelCommand::executeImpl ()
{
	Receiver->delete_file_or_link(Source.Path);
}
void tMakeDirCommand::executeImpl ()
{
	Receiver->create_dir(Source.Path);
}
void tChangeDirCommand::executeImpl ()
{
	Receiver->change_dir(Source.Path);
}
void tRemoveDirCommand::executeImpl ()
{
	Receiver->remove_dir(Source.Path);
}
void tRecursiveRemoveDirCommand::executeImpl ()
{
	Receiver->recursive_remove_dir(Source.Path);
}
void tMakeFileCommand::executeImpl ()
{
	Receiver->create_file(Source.Path);
}
void tMakeSoftLinkCommand::executeImpl ()
{
	Receiver->create_soft_link(Source.Path, Dest.Path);
}

void tMakeHardLinkCommand::executeImpl ()
{
	Receiver->create_hard_link(Source.Path, Dest.Path);
}
