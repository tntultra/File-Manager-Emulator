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
	
	tDir* get_path_parent(const std::string& path) const;
	tDir* get_dir_by_path(const std::string& path) const;
	bool path_represents_current_dir(const std::string& path) const;
	std::shared_ptr<tFileBase> get_file_by_path(const std::string& path) const;

public:
	tFileManager();
	tFileManager(const tFileManager&) = delete;
	tFileManager(tFileManager&&) = default;
	tFileManager& operator=(const tFileManager&) = delete;
	tFileManager& operator=(tFileManager&&) = default;
	~tFileManager() = default;


	void create_file(const std::string& path) const;
	void create_dir(const std::string& path) const;
	void change_dir(const std::string& path) const;
	void remove_dir(const std::string& path) const;
	void recursive_remove_dir(const std::string& path) const;
	void create_hard_link(const std::string& source, const std::string& dest) const;
	void create_soft_link(const std::string& source, const std::string& dest) const;
	void delete_file_or_link(const std::string& path) const;
	void move(const std::string& source, const std::string& dest) const;
	void copy(const std::string& source, const std::string& dest) const;
};

std::vector<std::string> split_path(const std::string& path);
std::string get_name(const std::string& path);
bool tome(const std::string& path);
bool has_extension(const std::string& path);
