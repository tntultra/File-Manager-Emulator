#include "stdafx.h"
#include "Dir.h"
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <queue>

bool operator< (tDir& lhs, tDir& rhs) {
	return lhs.Name < rhs.Name;
}

tDir::tDir (tDir* parent, const ci_string& name) :
	Parent{ parent }, 
	Name {name}
{
}

tDir::tDir (const tDir& rhs) :
	Parent (nullptr),
	Name(rhs.Name)
{
	std::for_each(rhs.Files.begin (),rhs.Files.end (),[this](auto&& fptr)
	{
		tFileBase* ptr;
		ptr.reset(fptr->clone());
		Files.push_back(ptr);
	});
}

tDir& tDir::operator= (tDir& rhs) {
	auto temp(rhs);
	std::swap(*this, temp);
	return *this;
}

bool tDir::has_hard_links() noexcept {
	return std::find_if(Links.begin(), Links.end(), [this](auto&& it)
	{
		return it->get_type() == FILEBASE_TYPE::HARD_LINK;
	}) != Links.end();
}

bool tDir::has_soft_links() noexcept {
	return std::find_if(Links.begin(), Links.end(), [this](auto&& it)
	{
		return it->get_type() == FILEBASE_TYPE::SOFT_LINK;
	}) != Links.end();
}

void tDir::remove_hard_link (tFileBase* hl)
{
	Links.erase(
		std::remove_if(Links.begin(), Links.end(), [&](auto&& exhl)
	{
		return exhl == hl;
	}));
}

void tDir::remove_soft_link (tFileBase* sl)
{
	Links.erase(
		std::remove_if(Links.begin(), Links.end(), [&](auto&& exsl)
	{
		return exsl == sl;
	}));
}

ci_string tDir::path ()
{
	auto parent = parent();
	auto path = get_name_impl();
	while (parent) {
		path = parent-> path() + '\\' + path;
	}
	return path;
}

bool tDir::empty ()
{
	return (Node-> DataBlock.size() <= 2);
}

bool tDir::can_be_removed ()
{
	std::queue<tDir*> dirs;
	dirs.push(this);
	while (!dirs.empty()) {
		auto dir = dirs.front();
		dirs.pop();
		if (dir-> hard_link_to_file_exists()) {
			return false;
		}
		for (auto& childDir : dir-> Dirs) {
			dirs.push(&childDir);
		}
	}
	return true;
}

bool tDir::tree_contains_dir (tDir* dirToFind)
{
	std::queue<tDir*> dirs;
	for (auto& childDir : Dirs) {
		dirs.push(&childDir);
	}
	while (!dirs.empty()) {
		auto dir = dirs.front();
		dirs.pop();
		if (dir == dirToFind) {
			return true;
		}
		for (auto& childDir : dir->Dirs) {
			dirs.push(&childDir);
		}
	}
	return false;
}

unsigned tDir::parent ()
{
	return Node-> DataBlock->get_node_by_name ("..");
}

tDir::D_IT tDir::internal_search_dir (tDir::D_IT start, tDir::D_IT end, const ci_string& dirName)
{
	return std::find_if(start, end,
		[&](auto&& dir) { return dir->name() == dirName; });
}

tDir::F_IT tDir::internal_search_file_or_link(tDir::F_IT start, tDir::F_IT end, const ci_string& fileName)
{
	return std::find_if(start, end,
		[&](auto&& file) { return file->name() == fileName; });
}

bool tDir::hard_link_to_file_exists (const ci_string& fileName)
{
	return std::find_if(Files.begin(), Files.end(),
		[&](auto&& f) { return f->get_type () == FILEBASE_TYPE::HARD_LINK && fileName[0] ? f->name() == fileName : true; }) != Files.end();
}

bool tDir::soft_link_to_file_exists(const ci_string& fileName)
{
	return std::find_if(Files.begin(), Files.end(),
		[&](auto&& f) { return f->get_type() == FILEBASE_TYPE::SOFT_LINK && f->name() == fileName; }) != Files.end();
}

ci_string tDir::get_name_impl () noexcept
{
	if (!parent) {
		return "C:";
	}
	return parent()-> Node-> DataBlock->get_name_by_id(Node->Id);
}

INode::INodeType tDir::get_type_impl () noexcept
{
	return Node->Type;
}

tDir* tDir::clone_impl () const
{
	return new tDir(*this);
}

tDir* tDir::get_dir_by_name (const ci_string& dirName)
{
	auto it = internal_search_dir(Dirs.begin(), Dirs.end(), dirName);
	if (it != Dirs.end()) {
		return &*it;
	}
	return nullptr;
}

tFileBase* tDir::get_file_or_link_by_file (tFileBase* file)
{
	auto it = std::find_if(Files.begin(), Files.end(),
		[&](auto&& f)
	{
		if (f.get_type() == FILEBASE_TYPE::REGULAR_FILE) {
			return &f == file;
		} else if (f.get_type() == FILEBASE_TYPE::SOFT_LINK){
			return dynamic_cast<tSoftLink*>(&f)->File == file;
		} else {
			return dynamic_cast<tHardLink*>(&f)->File == file;
		}
	});
	return (it != Files.end() ? &*it : nullptr);
}

tFileBase* tDir::get_file_by_name (const ci_string& fileName)
{
	auto it = internal_search_file_or_link(Files.begin(), Files.end(), fileName);
	if (it != Files.end()) {
		return &*it;
	}
	return nullptr;
}

std::vector<tFileBase*> tDir::get_all_files_by_name(const ci_string& fileName)
{
	std::vector<tFileBase*> result;
	auto start = internal_search_file_or_link(Files.begin (), Files.end(), fileName);
	for (; start != Files.end(); start = internal_search_file_or_link(start, Files.end(), fileName)) {
		result.push_back(&*start);
	}
	return result;
}

void tDir::create_dir (const ci_string& dirName)
{
	Dirs.push_back(tDir{ this, dirName });
	//DirIndex[dirName] = &Dirs.back ();
}

void tDir::remove_dir (const ci_string& dirName)
{
	auto it = std::find_if(Dirs.begin(), Dirs.end(),
		[&](auto&& dir) { return dir->name() == dirName; });

	if (it != Dirs.end () && it->empty ()) {
		Dirs.erase(it);
		//DirIndex.erase(dirName);
	} else {
		//some kind of notification, that dir doesnt contain other dir with such name
	}
}
void tDir::recursive_remove_dir (const ci_string& dirName)
{
	Dirs.erase(internal_search_dir(Dirs.begin(), Dirs.end(), dirName));
}
void tDir::create_file (const ci_string& fileName)
{
	Files.push_back(tFile{ fileName });
	//FileIndex[fileName] = &Files.back();
}
void tDir::remove_file (const ci_string& fileName)
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
void tDir::create_hard_link (tFileBase* file)
{
	if (file) {
		if (!hard_link_to_file_exists (file->name())) {
			Files.push_back(tHardLink { file });
		}
	}
}

void tDir::create_soft_link (tFileBase* file)
{
	if (file) {
		if (!soft_link_to_file_exists(file->name())) {
			Files.push_back(tSoftLink{ file });
		}
	}
}

void tDir::insert_other_dir (tDir* dir)
{
	Dirs.push_back(*dir-> clone());
}

void tDir::copy_file (const tFileBase& file)
{
	Files.push_back(*file.clone());
}

void tDir::move_file(tFileBase&& file)
{
	Files.push_back(file);
}
