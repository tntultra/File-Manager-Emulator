#pragma once

#include "FileBase.h"
#include <list>

class tHardLink;
class tSoftLink;

class tFile : public tFileBase
{
	std::string Name;
	std::list<tHardLink*> HardLinks;
	std::list<tSoftLink*> SoftLinks;

	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	tFile(const std::string name);

	virtual ~tFile() = default;
};

class tHardLink : public tFileBase
{
	tFile* File;
	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	tHardLink(const tFile* file);
	virtual ~tHardLink() = default;
};

class tSoftLink : public tFileBase
{
	tFile* File;
	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
public:
	tSoftLink(const tFile* file);
	virtual ~tSoftLink() = default;
};

