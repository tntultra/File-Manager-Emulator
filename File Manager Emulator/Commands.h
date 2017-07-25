#pragma once

#include "Command.h"

#include <vector>
#include <string>

class tMoveCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMoveCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tCopyCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tCopyCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tDelCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tDelCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
	 tCommand (receiver, parsedString) {}

};

class tMakeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeDirCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tChangeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tChangeDirCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tRemoveDirCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}
};

class tRecursiveRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override; 
public:
	explicit tRecursiveRemoveDirCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeFileCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeFileCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeHardLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeHardLinkCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}

};

class tMakeSoftLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeSoftLinkCommand(tFileManager* receiver, const std::vector<std::string>& parsedString) :
		tCommand(receiver, parsedString) {}
};
