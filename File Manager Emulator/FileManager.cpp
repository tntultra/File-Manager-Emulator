#include "stdafx.h"
#include "FileManager.h"
#include <list>
#include "FileBase.h"
#include "FileTypes.h"
#include <queue>

struct tFileManager::tFileManagerImpl
{
	std::shared_ptr<tDir> BaseDir{nullptr, 'C' };//[Name:]
	std::shared_ptr<tDir> CurrentDir{ BaseDir };
};

void tFileManager::create_file(const std::string& path)
{
	auto dir = get_dir_by_path(path);
	auto fileName = get_name(path);
	if (dir) {
		dir->create_file(fileName);
	}
}

void tFileManager::create_dir(const std::string& path)
{
	auto dir = get_dir_by_path(path);
	auto dirName = get_name(path);
	if (dir) {
		dir->create_dir(dirName);
	}
	
}

void tFileManager::change_dir(const std::string& path)
{
	auto dir = get_dir_by_path(path);
	if (dir) {
		pImpl-> CurrentDir = dir;
	}
}

void tFileManager::remove_dir(const std::string& path)
{
	if (path_represents_current_dir(path)) {
		return;
	}
	auto parent = get_path_parent(path);
	if (parent) {
		parent->remove_dir(get_name(path));
	}
}

void tFileManager::recursive_remove_dir(const std::string& path)
{
	if (path_represents_current_dir(path)) {
		return;
	}
	auto parent = get_path_parent(path);
	if (parent) {
		parent->recursive_remove_dir(get_name(path));
	}
}

void tFileManager::create_hard_link(const std::string& source, const std::string& dest)
{
	auto file = get_file_by_path(source);
	auto dirToInsertInto = get_dir_by_path(dest);
	if (file && dirToInsertInto && file-> get_type () == FILE_TYPE::REGULAR_FILE) {
		dirToInsertInto->create_hard_link(file);
	}
}

void tFileManager::create_soft_link(const std::string& source, const std::string& dest)
{
	auto file = get_file_by_path(source);
	auto dirToInsertInto = get_dir_by_path(dest);
	if (file && dirToInsertInto) {
		dirToInsertInto->create_soft_link(file);
	}

}

void tFileManager::delete_file_or_link(const std::string& path)
{
	if (has_extension (path)) {
		auto fileDir = get_dir_by_path(path);
		if (fileDir) {
			fileDir->remove_file(get_name(path));
		}
	} else {
		auto dirParent = get_path_parent(path);
		if (dirParent) {
			dirParent->remove_dir(get_name(path));
		}
	}
}

void tFileManager::move(const std::string& source, const std::string& dest)
{
	auto existingDir = get_dir_by_path(source);
	auto dirToMoveInto = get_dir_by_path(dest);
	if (existingDir && dirToMoveInto) {
		dirToMoveInto->insert_other_dir(std::move(*existingDir));
		get_path_parent(source)->remove_dir(get_name(source));
	}
}

void tFileManager::copy(const std::string& source, const std::string& dest)
{
	auto existingDir = get_dir_by_path(source);
	auto dirToMoveInto = get_dir_by_path(dest);
	if (existingDir && dirToMoveInto) {
		dirToMoveInto->insert_other_dir(*existingDir);
	}
}
