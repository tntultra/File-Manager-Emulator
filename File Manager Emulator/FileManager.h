#pragma once

#include <string>
#include <memory>

//model
class tFileManager
{
	struct tFileManagerImpl;
	std::unique_ptr<tFileManagerImpl> pImpl;
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