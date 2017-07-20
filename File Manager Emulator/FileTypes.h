#pragma once

#include "FileBase.h"

class tFile : public tFileBase
{
public:
	virtual ~tFile() = default;
};

class tDir : public tFileBase
{
public:
	virtual ~tDir() = default;
};

class tHardLink : public tFileBase
{
public:
	virtual ~tHardLink() = default;
};

class tSoftLink : public tFileBase
{
public:
	virtual ~tSoftLink() = default;
};