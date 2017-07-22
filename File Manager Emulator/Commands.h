#pragma once

#include "Command.h"

#include <vector>
#include <string>

class tMoveCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMoveCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tCopyCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tCopyCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tDelCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tDelCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tMakeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeDirCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tChangeDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tChangeDirCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tRemoveDirCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);
};

class tRecursiveRemoveDirCommand : public tCommand
{
	virtual void executeImpl() const override; 
public:
	explicit tRecursiveRemoveDirCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tMakeFileCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeFileCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tMakeHardLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeHardLinkCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);

};

class tMakeSoftLinkCommand : public tCommand
{
	virtual void executeImpl() const override;
public:
	explicit tMakeSoftLinkCommand(std::weak_ptr<tFileManager> receiver, const std::vector<std::string>& parsedString);
};
