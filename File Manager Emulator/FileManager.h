#pragma once

#include <string>
#include <memory>
#include <vector>
#include "FileTypes.h"
#include "Dir.h"

//model
class tFileManager
{
	struct tFileManagerImpl;
	std::unique_ptr<tFileManagerImpl> pImpl;
	
	tDir* get_path_parent(const std::string& path);
	tDir* get_dir_by_path(const std::string& path);
	bool path_represents_current_dir(const std::string& path);
	tFileBase* get_file_by_path(const std::string& path);

public:
	void create_file(const std::string& path);
	void create_dir(const std::string& path);
	void change_dir(const std::string& path);
	void remove_dir(const std::string& path);
	void recursive_remove_dir(const std::string& path);
	void create_hard_link(const std::string& source, const std::string& dest);
	void create_soft_link(const std::string& source, const std::string& dest);
	void delete_file_or_link(const std::string& path);
	void move(const std::string& source, const std::string& dest);
	void copy(const std::string& source, const std::string& dest);
};

std::vector<std::string> split_path(const std::string& path);
std::string get_name(const std::string& path);
bool has_extension(const std::string& path);
