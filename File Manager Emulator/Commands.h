#pragma once

#include "Command.h"

class tMoveCommand : public tCommand
{
	struct tMoveCommandImpl;
	uptr<tMoveCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tMoveCommand(const std::vector<std::string>& parsedString);

};

class tCopyCommand : public tCommand
{
	struct tCopyCommandImpl;
	uptr<tCopyCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tCopyCommand(const std::vector<std::string>& parsedString);

};

class tDelCommand : public tCommand
{
	struct tDelCommandImpl;
	uptr<tDelCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tDelCommand(const std::vector<std::string>& parsedString);

};

class tMakeDirCommand : public tCommand
{
	struct tMakeDirCommandImpl;
	uptr<tMakeDirCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tMakeDirCommand(const std::vector<std::string>& parsedString);

};

class tChangeDirCommand : public tCommand
{
	struct tChangeDirCommandImpl;
	uptr<tChangeDirCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tChangeDirCommand(const std::vector<std::string>& parsedString);

};

class tRemoveDirCommand : public tCommand
{
	struct tRemoveDirCommandImpl;
	uptr<tRemoveDirCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tRemoveDirCommand(const std::vector<std::string>& parsedString);
};

class tRecursiveRemoveDirCommand : public tCommand
{
	struct tRecursiveRemoveDirCommandImpl;
	uptr<tRecursiveRemoveDirCommandImpl> pImpl;

	virtual void executeImpl() const override; 
public:
	explicit tRecursiveRemoveDirCommand(const std::vector<std::string>& parsedString);

};

class tMakeFileCommand : public tCommand
{
	struct tMakeFileCommandImpl;
	uptr<tMakeFileCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tMakeFileCommand(const std::vector<std::string>& parsedString);

};

class tMakeHardLinkCommand : public tCommand
{
	struct tMakeHardLinkCommandImpl;
	uptr<tMakeHardLinkCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tMakeHardLinkCommand(const std::vector<std::string>& parsedString);

};

class tMakeSoftLinkCommand : public tCommand
{
	struct tMakeSoftLinkCommandImpl;
	uptr<tMakeSoftLinkCommandImpl> pImpl;

	virtual void executeImpl() const override;
public:
	explicit tMakeSoftLinkCommand(const std::vector<std::string>& parsedString);

};
