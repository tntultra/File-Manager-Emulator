#include "stdafx.h"
#include "FileManager.h"
#include "FileBase.h"
#include <queue>

tDir* tFileManager::get_dir_by_path(const std::vector<ci_string>& path, bool parent)
{
	if (!path.empty()) {
		auto pathHasFullName = tome(path.front());
		auto startDir = pathHasFullName ? &BaseDir : CurrentDir;
		auto end = has_extension(path) || parent ? path.end() - 1 : path.end();
		for (auto it = pathHasFullName ? path.begin()+1 : path.begin (); startDir && it != end; ++it) {
			startDir = startDir->get_dir_by_name(*it);
		}
		return startDir;
	}
	return nullptr;
}

/*bool tFileManager::path_represents_current_dir(const std::vector<ci_string>& path)
{
	return CurrentDir-> path () == path;
}*/

std::shared_ptr<tFileBase> tFileManager::get_file_by_path(const std::vector<ci_string>& path)
{
	if (!has_extension(path))
		return nullptr;
	auto fileDir = get_dir_by_path(path, false);
	if (!fileDir)
		return nullptr;
	return fileDir->get_file_by_name(get_name(path));
}

//mf 
void tFileManager::create_file(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	auto fileName = get_name(path);
	if (dir) {
		auto file = dir->get_file_by_name(fileName);
		if (!file)
			dir->create_file(fileName);
	}
}

//md
void tFileManager::create_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, true);
	auto dirName = get_name(path);
	if (dir) {
		dir->create_dir(dirName);
	}
}

//cd
void tFileManager::change_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	if (dir) {
		 CurrentDir = dir;
	}
}

//rd
void tFileManager::remove_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	if (dir ==  CurrentDir || !dir-> empty()) {
		return;
	}
	auto parent = dir-> parent ();
	if (parent) {
		parent->remove_dir(get_name(path));
	}
}

//deltree
void tFileManager::recursive_remove_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	if (!dir)
		return;
	auto parent = dir->parent();
	if (parent) {
		parent->recursive_remove_dir (get_name(path));
	}
}

//mhl
void tFileManager::create_hard_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto file = get_file_by_path(source);
	if (file &&  file->get_type() != FILE_TYPE::HARD_LINK &&  file->get_type() != FILE_TYPE::SOFT_LINK) {
		auto dirToInsertInto = get_dir_by_path(dest, false);
		if (dirToInsertInto) {
			dirToInsertInto->create_hard_link(file);
		}
	}
}

//mdl
void tFileManager::create_soft_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto file = get_file_by_path(source);
	if (file &&  file->get_type() != FILE_TYPE::HARD_LINK &&  file->get_type() != FILE_TYPE::SOFT_LINK) {
		auto dirToInsertInto = get_dir_by_path(dest, false);
		if (dirToInsertInto) {
			dirToInsertInto->create_soft_link(file);
		}
	}
}

void tFileManager::remove_all_file_soft_links (std::shared_ptr<tFileBase>& file)
{
	std::queue<tDir*> dirs;
	dirs.push(&BaseDir);
	while (!dirs.empty()) {
		auto someDir = dirs.front();
		dirs.pop();
		std::shared_ptr<tFileBase> foundFile;
		do {
			foundFile = someDir->get_file_or_link_by_file(file);
			if (foundFile && foundFile->get_type () == FILE_TYPE::SOFT_LINK) {
				someDir->remove_file(foundFile->name());
			}
		} while (foundFile);
		for (auto dir : someDir->Dirs) {
			dirs.push(&dir);
		}
	}
}

//del
void tFileManager::delete_file_or_link(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	if (dir) {
		auto file = dir->get_file_by_name(get_name(path));
		if (file) {
			auto ftype = file->get_type();
			if ((ftype == FILE_TYPE::REGULAR_FILE && dynamic_cast<tFile*>(file.get())->has_hard_links())
				|| ftype == FILE_TYPE::HARD_LINK) {
				return;
			}
			//can remove only dynamic link or regular file without hardlinks
			if (ftype == FILE_TYPE::REGULAR_FILE && dynamic_cast<tFile*>(file.get())->has_soft_links()) {
				remove_all_file_soft_links(file);
			}
		}
		//regular file without links or soft link -> simple remove from dir.
		dir->remove_file(get_name(path));
	}
}

//move
void tFileManager::move(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto dirToMoveInto = get_dir_by_path(dest, false);
	if (!has_extension(source)) {//source is actually a directory -> move directory
		auto existingDir = get_dir_by_path(source, false);
		if (existingDir && dirToMoveInto) {
			if (!existingDir->can_be_removed())
				throw std::runtime_error("moving directory with hardlink in it (or some of subdirectories)");
			dirToMoveInto->insert_other_dir(std::move(*existingDir));
			if (existingDir->parent())
				existingDir->parent()->remove_dir(get_name(source));
			existingDir->Parent = dirToMoveInto;
		}
	}
	else {
		auto existingFile = get_file_by_path(source);
		if (existingFile && dirToMoveInto) {
			auto existingDir = get_dir_by_path(source, false);
			dirToMoveInto->move_file(existingFile);
			existingDir->remove_file(get_name(source));
		}
	}
}

//copy
void tFileManager::copy(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto dirToMoveInto = get_dir_by_path(dest, false);
	if (!has_extension(source)) {
		auto existingDir = get_dir_by_path(source, false);
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

ci_string get_name(const std::vector<ci_string>& path)
{
	if (path.empty()) {
		return ci_string{};
	}
	return path.back();
}

bool tome(const ci_string& firstDir)
{
	return firstDir.find(':') != ci_string::npos;
}

bool has_extension (const std::vector<ci_string>& path)
{
	return get_name(path).rfind('.') != ci_string::npos;
}
