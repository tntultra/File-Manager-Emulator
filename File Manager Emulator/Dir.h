#pragma once

#include "FileTypes.h"

#include <memory>
#include <string>
#include <map>

class tDir
{
	tDir* Parent;
	std::string Name;
	std::list<tDir> Dirs;
	std::list<tFileBase> Files;

	std::map<std::string, tDir*> DirIndex;
	std::map<std::string, tFileBase*> FileIndex;

public:
	tDir(tDir* parent, const std::string& name);

	std::string name() const;
	bool empty() const;
	bool can_be_removed() const;

	tDir* parent() const;
	tDir* get_dir_by_name(const std::string& dirName) const;
	tFileBase* get_file_by_name(const std::string& fileName) const;

	void create_dir(const std::string& dirName);
	void remove_dir(const std::string& dirName);
	void recursive_remove_dir(const std::string& dirName);
	
	void create_file(const std::string& fileName);
	void remove_file(const std::string& fileName);

	void create_hard_link(const tFileBase* file);
	void create_soft_link(const tFileBase* file);

	void insert_other_dir(tDir&& dir);
	void insert_other_dir(const tDir& dir);

	friend bool operator< (const tDir&, const tDir&);
};

//void find_dir_in_dir(std::shared_ptr<tDir> dir, std::shared_ptr<tDir> findDir);

