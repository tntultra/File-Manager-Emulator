#include "stdafx.h"
#include "FileTypes.h"
#include <list>
#include <algorithm>

std::string tFile::get_name_impl () const noexcept {
	return Name;
}

FILE_TYPE tFile::get_type_impl () const noexcept {
	return FILE_TYPE::REGULAR_FILE;
}

void tFile::remove_hard_link (std::shared_ptr<tFileBase> hl)
{
	Links.erase (
		std::remove_if (Links.begin(), Links.end(),[&](auto&& exhl)
	{
		return exhl == hl;
	}));
}

void tFile::remove_soft_link (std::shared_ptr<tFileBase> sl)
{
	Links.erase(
		std::remove_if(Links.begin(), Links.end(), [&](auto&& exsl)
	{
		return exsl == sl;
	}));
}

std::string tHardLink::get_name_impl () const noexcept {
	return File->name();
}

FILE_TYPE tHardLink::get_type_impl () const noexcept {
	return FILE_TYPE::HARD_LINK;
}

tHardLink::tHardLink (std::shared_ptr<tFileBase> file) :
	File (file)
{
}

std::string tSoftLink::get_name_impl() const noexcept {
	return File->name ();
}

FILE_TYPE tSoftLink::get_type_impl () const noexcept {
	return FILE_TYPE::SOFT_LINK;
}

tSoftLink::tSoftLink(std::shared_ptr<tFileBase> file) :
	File(file)
{
}

std::unique_ptr<tFileBase> tFile::clone_impl() const
{
	return std::make_unique<tFile>(tFile{ Name });
}

std::unique_ptr<tFileBase> tHardLink::clone_impl() const
{
	return std::make_unique<tHardLink>(tHardLink{ File });
}

std::unique_ptr<tFileBase> tSoftLink::clone_impl () const
{
	return std::make_unique<tSoftLink>(tSoftLink{ File });
}
