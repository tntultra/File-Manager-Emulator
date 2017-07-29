#pragma once

#include "FileTypes.h"

#include <memory>
#include <string>
#include <map>
#include <vector>

class tDir
{
	friend class tFileManager;
	using F_IT = std::list<std::shared_ptr<tFileBase>>::iterator;
	using D_CIT = std::list<tDir>::iterator;

	tDir* Parent;
	std::string Name;
	std::list<tDir> Dirs;
	std::list<std::shared_ptr<tFileBase> > Files;

	D_CIT internal_search_dir(D_CIT start, D_CIT end, const std::string& dirName);//should return optional

	F_IT internal_search_file_or_link(F_IT start, F_IT end, const std::string& fileName);

	bool hard_link_to_file_exists(const std::string& fileName = "");//blank means any hardlink, special behavior for move operation
	bool soft_link_to_file_exists(const std::string& fileName);

public:
	tDir(tDir* parent, const std::string& name);
	tDir(tDir&&) = default;
	tDir(const tDir&);
	tDir& operator=(tDir&&) = default;
	tDir& operator=(tDir&);
	~tDir() = default;

	std::string path();
	std::string name();
	bool empty();
	bool can_be_removed();

	tDir* parent();
	tDir* get_dir_by_name(const std::string& dirName);//add version
	std::shared_ptr<tFileBase> get_file_or_link_by_file(std::shared_ptr<tFileBase> file);
	std::shared_ptr<tFileBase> get_file_by_name(const std::string& fileName);//add version
	std::vector<std::shared_ptr<tFileBase>> get_all_files_by_name(const std::string& fileName);

	void create_dir(const std::string& dirName);
	void remove_dir(const std::string& dirName);
	void recursive_remove_dir(const std::string& dirName);
	
	void create_file(const std::string& fileName);
	void remove_file(const std::string& fileName);//simply removes file (or link) from dir.
	//filemanager handles all dynamic links that are associated with it

	void create_hard_link(std::shared_ptr<tFileBase> file);
	void create_soft_link(std::shared_ptr<tFileBase> file);

	void insert_other_dir(tDir&& dir);
	void insert_other_dir(tDir& dir);

	void copy_file(std::shared_ptr<tFileBase> file);//all of the following functions work for links also
	void move_file(std::shared_ptr<tFileBase> file);


	friend bool operator< (tDir&, tDir&);
};

//void find_dir_in_dir(std::shared_ptr<tDir> dir, std::shared_ptr<tDir> findDir);

