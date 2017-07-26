#include "stdafx.h"
#include "Dir.h"
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <queue>

bool operator< (const tDir& lhs, const tDir& rhs) {
	return lhs.Name < rhs.Name;
}

tDir::tDir (tDir* parent, const std::string& name) :
	Parent{ parent }, 
	Name {name}
{
}

tDir::tDir (const tDir& rhs) :
	Parent (nullptr),
	Dirs (rhs.Dirs),
	Name (rhs.Name)
{
	std::for_each(rhs.Files.begin (),rhs.Files.end (),[this](auto&& fptr)
	{
		Files.push_back(fptr->clone());
	});
}

tDir& tDir::operator= (const tDir& rhs) {
	auto temp(rhs);
	std::swap(*this, temp);
	return *this;
}

std::string tDir::path () const
{
	auto parent = Parent;
	auto path = name ();
	while (parent) {
		path = parent-> path() + path;
	}
	return path;
}

std::string tDir::name () const
{
	return Name;
}

bool tDir::empty () const
{
	return (Dirs.size() + Files.size() == 0);
}

bool tDir::can_be_removed () const
{
	std::queue<const tDir*> dirs;
	dirs.push(this);
	while (!dirs.empty()) {
		auto dir = dirs.front();
		dirs.pop();
		if (dir-> hard_link_to_file_exists()) {
			return true;
		}
		for (auto& childDir : dir-> Dirs) {
			dirs.push(&childDir);
		}
	}
	return false;
}

tDir* tDir::parent () const
{
	return Parent;
}

tDir::D_CIT tDir::internal_search_dir (tDir::D_CIT start, tDir::D_CIT end, const std::string& dirName) const
{
	return std::find_if(start, end,
		[&](auto&& dir) { return dir.name() == dirName; });
}

tDir::F_CIT tDir::internal_search_file_or_link(tDir::F_CIT start, tDir::F_CIT end, const std::string& fileName) const
{
	return std::find_if(start, end,
		[&](auto&& file) { return file->name() == fileName; });
}

bool tDir::hard_link_to_file_exists (const std::string& fileName) const
{
	return std::find_if(Files.begin(), Files.end(),
		[&](auto&& f) { return f->get_type () == FILE_TYPE::HARD_LINK && fileName[0] ? f->name() == fileName : true; }) != Files.end();
}

bool tDir::soft_link_to_file_exists(const std::string& fileName) const
{
	return std::find_if(Files.begin(), Files.end(),
		[&](auto&& f) { return f->get_type() == FILE_TYPE::SOFT_LINK && f->name() == fileName; }) != Files.end();
}

const tDir* tDir::get_dir_by_name (const std::string& dirName) const
{
	auto it = internal_search_dir(Dirs.begin(), Dirs.end(), dirName);
	if (it != Dirs.end()) {
		return &(*it);
	}
	return nullptr;
}

std::shared_ptr<tFileBase> tDir::get_file_or_link_by_file (std::shared_ptr<tFileBase> file) const
{
	auto it = std::find_if(Files.begin(), Files.end(),
		[&](auto&& f)
	{
		if (f.get_type() == FILE_TYPE::REGULAR_FILE) {
			return f.get() == file.get();
		} else if (f.get_type() == FILE_TYPE::SOFT_LINK){
			auto wptrFile = dynamic_cast<tSoftLink*>(f.get())->File;
			if (!wptrFile.expired()) {
				return wptrFile.lock()->File.get() == file.get();
			}
			return false;
		} else {
			auto wptrFile = dynamic_cast<tHardLink*>(f.get())->File;
			if (!wptrFile.expired()) {
				return wptrFile.lock()->File.get() == file.get();
			}
			return false;
		}
	});
	return (it != Files.end() ? *it : nullptr);
}

std::shared_ptr<tFileBase> tDir::get_file_by_name (const std::string& fileName) const
{
	auto it = internal_search_file_or_link(Files.begin(), Files.end(), fileName);
	if (it != Files.end()) {
		return *it;
	}
	return nullptr;
}

std::vector<std::shared_ptr<tFileBase>> tDir::get_all_files_by_name(const std::string& fileName) const
{
	std::vector<std::shared_ptr<tFileBase>> result;
	auto start = internal_search_file_or_link(Files.begin (), Files.end(), fileName);
	for (; start != Files.end(); start = internal_search_file_or_link(start, Files.end(), fileName)) {
		result.push_back(*start);
	}
	return result;
}

void tDir::create_dir (const std::string& dirName)
{
	Dirs.push_back(tDir{ this, dirName });
	//DirIndex[dirName] = &Dirs.back ();
}

void tDir::remove_dir (const std::string& dirName)
{
	auto it = std::find_if(Dirs.begin(), Dirs.end(),
		[&](auto&& dir) { return dir.name() == dirName; });

	if (it != Dirs.end () && it-> empty ()) {
		Dirs.erase(it);
		//DirIndex.erase(dirName);
	} else {
		//some kind of notification, that dir doesnt contain other dir with such name
	}
}
void tDir::recursive_remove_dir (const std::string& dirName)
{
	Dirs.erase(internal_search_dir(Dirs.begin(), Dirs.end(), dirName));
}
void tDir::create_file (const std::string& fileName)
{
	Files.push_back(std::make_shared<tFile>(fileName ));
	//FileIndex[fileName] = &Files.back();
}
void tDir::remove_file (const std::string& fileName)
{
	auto it = internal_search_file_or_link(Files.begin(), Files.end(), fileName);
	if (it != Files.end()) {
		Files.erase(it);
		//FileIndex.erase(fileName);
	}
	else {
		//some kind of notification, that dir doesnt contain file with such name
	}
}
void tDir::create_hard_link (std::shared_ptr<tFileBase> file)
{
	if (file) {
		if (!hard_link_to_file_exists (file->name())) {
			Files.push_back(std::make_shared<tHardLink>( file ));
		}
	}
}

void tDir::create_soft_link (std::shared_ptr<tFileBase> file)
{
	if (file) {
		if (!soft_link_to_file_exists(file->name())) {
			Files.push_back(std::make_shared<tSoftLink>( file ));
		}
	}
}

void tDir::insert_other_dir (tDir&& dir)
{
	Dirs.emplace_back(dir);//because dir is rvalue
	//DirIndex[dir.name()] = &Dirs.back();
}

void tDir::insert_other_dir (const tDir& dir)
{
	Dirs.push_back(dir);
	//DirIndex[dir.name()] = &Dirs.back();
}

void tDir::copy_file (std::shared_ptr<tFileBase> file)
{
	Files.push_back(file->clone());
}

void tDir::move_file(std::shared_ptr<tFileBase> file)
{
	Files.push_back(file);
}
