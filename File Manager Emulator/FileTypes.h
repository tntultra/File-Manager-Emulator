#pragma once

#include "FileBase.h"

class tFile : public tFileBase
{
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	virtual ~tFile() = default;
};

class tHardLink : public tFileBase
{
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	virtual ~tHardLink() = default;
};

class tSoftLink : public tFileBase
{
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	virtual ~tSoftLink() = default;
};

