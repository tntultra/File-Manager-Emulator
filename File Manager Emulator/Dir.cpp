#include "stdafx.h"
#include "Dir.h"
#include <map>
#include <set>
#include <list>
#include <algorithm>

bool operator< (const tDir& lhs, const tDir& rhs) {
	return lhs.Name < rhs.Name;
}

tDir::tDir (tDir* parent, const std::string& name) :
	Parent{ parent }, 
	Name {name}
{
}

std::string tDir::name () const
{
	return Name;
}

bool tDir::empty () const
{
	return (Dirs.size() + Files.size() == 0);
}

bool tDir::can_be_removed () const {}

tDir* tDir::parent () const
{
	return Parent;
}

tDir* tDir::get_dir_by_name (const std::string& dirName) const
{
	auto it = DirIndex.find(dirName);
	if (it != DirIndex.end()) {
		return it->second;
	}
	return nullptr;
}

tFileBase* tDir::get_file_by_name (const std::string& fileName) const
{
	auto it = FileIndex.find(fileName);
	if (it != FileIndex.end()) {
		return it->second;
	}
	return nullptr;
}

void tDir::create_dir (const std::string& dirName)
{
	Dirs.push_back(tDir{ this, dirName });
	DirIndex[dirName] = &Dirs.back ();
}

void tDir::remove_dir (const std::string& dirName)
{
	auto it = std::find_if(Dirs.begin(), Dirs.end(),
		[&](auto&& dir) { return dir.name() == dirName; });

	if (it != Dirs.end ()) {
		Dirs.erase(it);
		DirIndex.erase(dirName);
	} else {
		//some kind of notification, that dir doesnt contain other dir with such name
	}
}
void tDir::recursive_remove_dir (const std::string& dirName)
{
	
}
void tDir::create_file (const std::string& fileName)
{
	Files.push_back(tFile{fileName });
	FileIndex[fileName] = &Files.back();
}
void tDir::remove_file (const std::string& fileName)
{
	auto it = std::find_if(Files.begin(), Files.end(),
		[&](auto&& file) { return file.name() == fileName; });

	if (it != Files.end()) {
		Files.erase(it);
		FileIndex.erase(fileName);
	}
	else {
		//some kind of notification, that dir doesnt contain file with such name
	}
}
void tDir::create_hard_link (const tFileBase* file)
{
	if (!file) {
		auto it = FileIndex.find(file-> name());
		if (it == FileIndex.end() && it-> second-> get_type () != FILE_TYPE::HARD_LINK ) {
			Files.push_back(tHardLink{ file });
		}
	}
}

void tDir::create_soft_link (const tFileBase* file)
{
	if (!file) {
		auto it = std::find_if(Files.begin(), Files.end(),
			[&](auto&& fileInDir) { return fileInDir.name() == file->name() && fileInDir.get_type() == FILE_TYPE::SOFT_LINK; });
		if (it == Files.end()) {
			Files.push_back(tSoftLink{ file });
		}
	}
}

void tDir::insert_other_dir (tDir&& dir)
{
	Dirs.emplace_back(dir);//because dir is rvalue
	DirIndex[dir.name()] = &Dirs.back();
}

void tDir::insert_other_dir (const tDir& dir)
{
	Dirs.push_back(dir);
	DirIndex[dir.name()] = &Dirs.back();
}
