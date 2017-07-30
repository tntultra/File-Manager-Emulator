#pragma once

#include "Command.h"

#include <vector>
#include <string>

class tMoveCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tMoveCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tCopyCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tCopyCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tDelCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tDelCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
	 tCommand (receiver, parsedString) {}

};

class tMakeDirCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tMakeDirCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tChangeDirCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tChangeDirCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tRemoveDirCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tRemoveDirCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}
};

class tRecursiveRemoveDirCommand : public tCommand
{
	virtual void executeImpl() override; 
public:
	explicit tRecursiveRemoveDirCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeFileCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tMakeFileCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeHardLinkCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tMakeHardLinkCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeSoftLinkCommand : public tCommand
{
	virtual void executeImpl() override;
public:
	explicit tMakeSoftLinkCommand(tFileManager* receiver, std::vector<ci_string>& parsedString) :
		tCommand(receiver, parsedString) {}
};
