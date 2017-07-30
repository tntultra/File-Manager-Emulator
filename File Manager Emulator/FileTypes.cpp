#include "stdafx.h"
#include "FileTypes.h"
#include <list>
#include <algorithm>

ci_string tFile::get_name_impl () noexcept {
	return Name;
}

FILE_TYPE tFile::get_type_impl () noexcept {
	return FILE_TYPE::REGULAR_FILE;
}

bool tFile::has_hard_links() noexcept {
	return std::find_if(Links.begin(), Links.end(), [this](auto&& it)
	{
		return it->get_type() == FILE_TYPE::HARD_LINK;
	}) != Links.end();
}

bool tFile::has_soft_links () noexcept {
	return std::find_if(Links.begin(), Links.end(), [this](auto&& it)
	{
		return it->get_type() == FILE_TYPE::SOFT_LINK;
	}) != Links.end();
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

ci_string tHardLink::get_name_impl () noexcept {
	if (!File.expired()) {
		return "hlink[" + File.lock()->name() + "]";
	}
	return "";
}

FILE_TYPE tHardLink::get_type_impl () noexcept {
	return FILE_TYPE::HARD_LINK;
}

tHardLink::tHardLink (std::shared_ptr<tFileBase> file) :
	File (file)
{
}

ci_string tSoftLink::get_name_impl() noexcept {
	if (!File.expired()) {
		return "dlink[" + File.lock()->name() + "]";
	}
	return "";
}

FILE_TYPE tSoftLink::get_type_impl () noexcept {
	return FILE_TYPE::SOFT_LINK;
}

tSoftLink::tSoftLink(std::shared_ptr<tFileBase> file) :
	File(file)
{
}

std::unique_ptr<tFileBase> tFile::clone_impl()
{
	return std::make_unique<tFile>(tFile{ Name });
}

std::unique_ptr<tFileBase> tHardLink::clone_impl()
{
	if (!File.expired()) {
		return std::make_unique<tHardLink>(tHardLink{ File.lock() });
	}
	return nullptr;
}

std::unique_ptr<tFileBase> tSoftLink::clone_impl ()
{
	if (!File.expired()) {
		return std::make_unique<tSoftLink>(tSoftLink{ File.lock() });
	}
	return nullptr;
}
