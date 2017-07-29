#pragma once

#include <string>
#include <memory>
#include <vector>
#include "FileTypes.h"
#include "Dir.h"

//model
class tFileManager
{
	tDir BaseDir{ nullptr, "C:" };//[Name:]
	tDir* CurrentDir{ &BaseDir };

	tDir* get_dir_by_path(const std::vector<std::string>& path, bool parent);
	bool path_represents_current_dir(const std::vector<std::string>& path);
	std::shared_ptr<tFileBase> get_file_by_path(const std::vector<std::string>& path);

public:
	tFileManager() = default;
	tFileManager(const tFileManager&) = delete;
	tFileManager(tFileManager&&) = default;
	tFileManager& operator=(const tFileManager&) = delete;
	tFileManager& operator=(tFileManager&&) = default;
	~tFileManager() = default;


	void create_file(const std::vector<std::string>& path);
	void create_dir(const std::vector<std::string>& path);
	void change_dir(const std::vector<std::string>& path);
	void remove_dir(const std::vector<std::string>& path);
	void recursive_remove_dir(const std::vector<std::string>& path);
	void create_hard_link(const std::vector<std::string>& source, const std::vector<std::string>& dest);
	void create_soft_link(const std::vector<std::string>& source, const std::vector<std::string>& dest);
	void remove_all_file_soft_links (std::shared_ptr<tFileBase>& file);
	void delete_file_or_link(const std::vector<std::string>& path);
	void move(const std::vector<std::string>& source, const std::vector<std::string>& dest);
	void copy(const std::vector<std::string>& source, const std::vector<std::string>& dest);
};

std::string get_name(const std::vector<std::string>& path);
bool tome(const std::string& path);
bool has_extension(const std::vector<std::string>& path);
