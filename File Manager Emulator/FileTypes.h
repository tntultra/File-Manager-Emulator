#pragma once

#include "FileBase.h"
#include <list>

class tHardLink;
class tSoftLink;

class tFile : public tFileBase
{
	friend class tHardLink;
	friend class tSoftLink;
	std::string Name;
	std::list<std::shared_ptr<tFileBase> > Links;

	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;
	void remove_hard_link(std::shared_ptr<tFileBase> hl);
	void remove_soft_link(std::shared_ptr<tFileBase> sl);
public:
	explicit tFile(const std::string name) : Name (name) {}

	virtual ~tFile() = default;
};

class tHardLink : public tFileBase
{
	//while link exists file exists. weird assumption, which doesnt meet real world, but that's the task
	//normally hardlink is just a reference to file and if you delete hard link reference counter of some structure associated with actual data on disk subtracts by 1 and data is intact.
	std::shared_ptr<tFileBase> File;
	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;
public:
	explicit tHardLink(std::shared_ptr<tFileBase>);
	virtual ~tHardLink() = default;
};

class tSoftLink : public tFileBase
{
	std::shared_ptr<tFileBase> File;
	//while link exists file exists. even more weird.
	//normally shortcuts just dangle upon delete operation.

	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;
public:
	explicit tSoftLink(std::shared_ptr<tFileBase>);
	virtual ~tSoftLink() = default;
};

