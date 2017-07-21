#pragma once

#include "Command.h"

#include <vector>
#include <string>

class tMoveCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMoveCommand(const std::vector<std::string>& parsedString);

};

class tCopyCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tCopyCommand(const std::vector<std::string>& parsedString);

};

class tDelCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tDelCommand(const std::vector<std::string>& parsedString);

};

class tMakeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeDirCommand(const std::vector<std::string>& parsedString);

};

class tChangeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tChangeDirCommand(const std::vector<std::string>& parsedString);

};

class tRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tRemoveDirCommand(const std::vector<std::string>& parsedString);
};

class tRecursiveRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override; 
public:
	explicit tRecursiveRemoveDirCommand(const std::vector<std::string>& parsedString);

};

class tMakeFileCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeFileCommand(const std::vector<std::string>& parsedString);

};

class tMakeHardLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeHardLinkCommand(const std::vector<std::string>& parsedString);

};

class tMakeSoftLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeSoftLinkCommand(const std::vector<std::string>& parsedString);
};
