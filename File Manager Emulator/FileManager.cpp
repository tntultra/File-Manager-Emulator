#include "stdafx.h"
#include "FileManager.h"
#include <list>
#include "FileBase.h"
#include "FileTypes.h"
#include <queue>

struct tFileManager::tFileManagerImpl
{
	std::shared_ptr<tDir> BaseDir{ new tDir{'C'} };//[Name:]
	std::shared_ptr<tDir> CurrentDir{ BaseDir };
};

void tFileManager::create_file(const std::string& path)
{
	auto dir = move_dir_to_path_location(path);
	auto fileName = get_name(path);
	if (dir) {
		dir->create_file(fileName);
	}
}

void tFileManager::create_dir(const std::string& path)
{
	auto dir = move_dir_to_path_location(path);
	auto dirName = get_name(path);
	if (dir) {
		dir->create_dir(dirName);
	}
	
}

void tFileManager::change_dir(const std::string& path)
{
	auto dir = move_dir_to_path_location(path);
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
	auto sourceDir = move_dir_to_path_location(source);
	auto sourceFileName = get_name(source);
	if (sourceDir) {
		auto wsourceFile = sourceDir->get_file_by_name(sourceFileName);
		if (!wsourceFile.expired()) {
			auto destDir = move_dir_to_path_location(dest);
			if (destDir) {
				destDir->create_hard_link(wsourceFile);
			}
		}
	}
}

void tFileManager::create_soft_link(const std::string& source, const std::string& dest)
{
}

void tFileManager::delete_file_or_link(const std::string& path)
{
}

void tFileManager::move(const std::string& source, const std::string& dest)
{
}

void tFileManager::copy(const std::string& source, const std::string& dest)
{
}
