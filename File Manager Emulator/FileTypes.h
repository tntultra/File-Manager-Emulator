#pragma once

#include "FileBase.h"
#include <list>
#include <algorithm>

class tHardLink;
class tSoftLink;

class tFile : public tFileBase
{
	std::string Name;
	std::list<std::shared_ptr<tFileBase> > Links;

	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;

public:
	explicit tFile(const std::string name) : Name (name) {}

	bool has_hard_links() const noexcept;
	bool has_soft_links() const noexcept;
	void remove_hard_link(std::shared_ptr<tFileBase> hl);
	void remove_soft_link(std::shared_ptr<tFileBase> sl);

	virtual ~tFile() = default;
};

class tHardLink : public tFileBase
{
	friend class tDir;
	//while link exists file exists. weird assumption, which doesnt meet real world, but that's the task
	//normally hardlink is just a reference to file and if you delete hard link reference counter of some structure associated with actual data on disk subtracts by 1 and data is intact.
	std::weak_ptr<tFileBase> File;
	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;
public:
	explicit tHardLink(std::shared_ptr<tFileBase>);
	virtual ~tHardLink() = default;
};

class tSoftLink : public tFileBase
{
	friend class tDir;
	std::weak_ptr<tFileBase> File;
	//while link exists file exists. even more weird.
	//normally shortcuts just dangle upon delete operation.

	virtual std::string get_name_impl() const noexcept override;
	virtual FILE_TYPE get_type_impl() const noexcept override;
	virtual std::unique_ptr<tFileBase> clone_impl() const override;
public:
	explicit tSoftLink(std::shared_ptr<tFileBase>);
	virtual ~tSoftLink() = default;
};

