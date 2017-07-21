#pragma once

#include "FileTypes.h"

#include <memory>
#include <string>

class tDir
{
	struct tDirImpl;
	std::unique_ptr<tDirImpl> pImpl;
public:
	std::string name() const;
	bool empty() const;
	bool can_be_removed() const;

	std::weak_ptr<tDir> parent() const;
	std::weak_ptr<tDir> get_dir_by_name(const std::string& dirName) const;
	std::weak_ptr<tFile> get_file_by_name(const std::string& dirName) const;

	void create_dir(const std::string& dirName);
	void remove_dir(const std::string& dirName);
	void recursive_remove_dir(const std::string& dirName);
	
	void create_file(const std::string& fileName);
	void remove_file(const std::string& fileName);

	void create_hard_link(std::weak_ptr<tFile> file);
	void create_soft_link(std::weak_ptr<tFile> file);
};

//void find_dir_in_dir(std::shared_ptr<tDir> dir, std::shared_ptr<tDir> findDir);

