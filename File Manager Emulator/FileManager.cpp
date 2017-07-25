#include "stdafx.h"
#include "FileManager.h"
#include <list>
#include "FileBase.h"
#include "FileTypes.h"
#include <queue>

struct tFileManager::tFileManagerImpl
{
	std::shared_ptr<tDir> BaseDir{nullptr, 'C' };//[Name:]
	tDir* CurrentDir{ BaseDir.get () };
};

tDir* tFileManager::get_dir_by_path (const std::string& path) const
{
	auto fullPath = split_path(path);
	if (!fullPath.empty()) {
		auto pathHasFullName = tome(fullPath.front());
		auto startDir = pathHasFullName ? pImpl->BaseDir.get() : pImpl->CurrentDir;
		auto end = has_extension(path) ? fullPath.end() - 1 : fullPath.end();
		for (auto it = pathHasFullName ? fullPath.begin() : fullPath.begin ()+1; startDir && it != end; ++it) {
			startDir = startDir->get_dir_by_name(*it);
		}
		return startDir;
	}
	return nullptr;
}

bool tFileManager::path_represents_current_dir (const std::string& path) const
{
	return pImpl->CurrentDir-> path () == path;
}

std::shared_ptr<tFileBase> tFileManager::get_file_by_path (const std::string& path) const
{
	if (!has_extension(path))
		return nullptr;
	auto fileDir = get_dir_by_path(path);
	if (!fileDir)
		return nullptr;
	return fileDir->get_file_by_name(get_name(path));
}





tFileManager::tFileManager() :
	pImpl{std::unique_ptr<tFileManagerImpl> ()}
{
}


//mf 
void tFileManager::create_file(const std::string& path)const
{
	auto dir = get_dir_by_path(path);
	auto fileName = get_name(path);
	if (dir) {
		if (!dir->get_file_by_name(fileName))
			dir->create_file(fileName);
	}
}

//md
void tFileManager::create_dir(const std::string& path) const
{
	auto dir = get_dir_by_path(path);
	auto dirName = get_name(path);
	if (dir) {
		dir->create_dir(dirName);
	}
}

//cd
void tFileManager::change_dir(const std::string& path) const
{
	auto dir = get_dir_by_path(path);
	if (dir) {
		pImpl-> CurrentDir = dir;
	}
}

//rd
void tFileManager::remove_dir(const std::string& path) const
{
	auto dir = get_dir_by_path(path);
	if (dir == pImpl-> CurrentDir || !dir-> empty()) {
		return;
	}
	auto parent = dir-> parent ();
	if (parent) {
		parent->remove_dir(get_name(path));
	}
}

//deltree
void tFileManager::recursive_remove_dir(const std::string& path) const
{
	auto dir = get_dir_by_path(path);
	if (!dir)
		return;
	auto parent = dir->parent();
	if (parent) {
		parent->recursive_remove_dir (get_name(path));
	}
}

//mhl
void tFileManager::create_hard_link(const std::string& source, const std::string& dest) const
{
	auto file = get_file_by_path(source);
	if (file &&  file->get_type() != FILE_TYPE::HARD_LINK &&  file->get_type() != FILE_TYPE::SOFT_LINK) {
		auto dirToInsertInto = get_dir_by_path(dest);
		if (dirToInsertInto) {
			dirToInsertInto->create_hard_link(file);
		}
	}
}

//mdl
void tFileManager::create_soft_link(const std::string& source, const std::string& dest) const
{
	auto file = get_file_by_path(source);
	if (file &&  file->get_type() != FILE_TYPE::HARD_LINK &&  file->get_type() != FILE_TYPE::SOFT_LINK) {
		auto dirToInsertInto = get_dir_by_path(dest);
		if (dirToInsertInto) {
			dirToInsertInto->create_soft_link(file);
		}
	}
}

//del
void tFileManager::delete_file_or_link(const std::string& path) const
{
	auto dir = get_dir_by_path(path);
	if (dir)
		dir->remove_file(get_name(path));
}

//move
void tFileManager::move(const std::string& source, const std::string& dest) const
{
	auto dirToMoveInto = get_dir_by_path(dest);
	if (!has_extension(source)) {//source is actually a directory -> move directory
		auto existingDir = get_dir_by_path(source);
		if (existingDir && dirToMoveInto) {
			dirToMoveInto->insert_other_dir(std::move(*existingDir));
			if (existingDir->parent())
				existingDir->parent()->remove_dir(get_name(source));
			existingDir->Parent = dirToMoveInto;
		}
	}
	else {
		auto existingFile = get_file_by_path(source);
		if (existingFile && dirToMoveInto) {
			auto existingDir = get_dir_by_path(source);
			dirToMoveInto->move_file(existingFile);
			existingDir->remove_file(get_name(source));
		}
	}
}

//copy
void tFileManager::copy(const std::string& source, const std::string& dest) const
{
	auto dirToMoveInto = get_dir_by_path(dest);
	if (!has_extension(source)) {
		auto existingDir = get_dir_by_path(source);
		if (existingDir && dirToMoveInto) {
			dirToMoveInto->insert_other_dir(*existingDir);
		}
	} else {
		auto existingFile = get_file_by_path(source);
		if (existingFile && dirToMoveInto) {
			dirToMoveInto->copy_file(existingFile);
		}
	}
}

std::vector<std::string> split_path (const std::string& path)
{
	const char slash = '\\';
	std::vector<std::string> splits;
	size_t begin = 0, slash_pos;
	do {
		slash_pos = path.find(slash);
		splits.push_back(path.substr(begin, slash_pos-begin));
		begin = slash_pos + 1;
	} while (slash_pos != std::string::npos);
	return splits;
}

std::string get_name (const std::string& path)
{
	auto lastSlash = path.rfind('\\');
	if (lastSlash == std::string::npos) {
		return path;
	}
	return path.substr(lastSlash);
}

bool tome (const std::string& firstDir)
{
	return firstDir.find(':') != std::string::npos;
}

bool has_extension (const std::string& path)
{
	return path.rfind('.') != std::string::npos;
}
