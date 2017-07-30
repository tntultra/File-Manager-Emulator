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

	tDir* get_dir_by_path(const std::vector<ci_string>& path, bool parent);
	bool path_represents_current_dir(const std::vector<ci_string>& path);
	std::shared_ptr<tFileBase> get_file_by_path(const std::vector<ci_string>& path);

public:
	tFileManager() = default;
	tFileManager(const tFileManager&) = delete;
	tFileManager(tFileManager&&) = default;
	tFileManager& operator=(const tFileManager&) = delete;
	tFileManager& operator=(tFileManager&&) = default;
	~tFileManager() = default;


	void create_file(const std::vector<ci_string>& path);
	void create_dir(const std::vector<ci_string>& path);
	void change_dir(const std::vector<ci_string>& path);
	void remove_dir(const std::vector<ci_string>& path);
	void recursive_remove_dir(const std::vector<ci_string>& path);
	void create_hard_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest);
	void create_soft_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest);
	void remove_all_file_soft_links (std::shared_ptr<tFileBase>& file);
	void delete_file_or_link(const std::vector<ci_string>& path);
	void move(const std::vector<ci_string>& source, const std::vector<ci_string>& dest);
	void copy(const std::vector<ci_string>& source, const std::vector<ci_string>& dest);
};

ci_string get_name(const std::vector<ci_string>& path);
bool tome(const ci_string& path);
bool has_extension(const std::vector<ci_string>& path);
