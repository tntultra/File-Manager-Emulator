#pragma once

#include "FileTypes.h"

#include <memory>
#include <string>
#include <map>
#include <vector>
#include "CIString.h"

class tDir
{
	friend class tFileManager;
	using F_IT = std::list<std::shared_ptr<tFileBase>>::iterator;
	using D_CIT = std::list<tDir>::iterator;

	tDir* Parent;
	ci_string Name;
	std::list<tDir> Dirs;
	std::list<std::shared_ptr<tFileBase> > Files;

	D_CIT internal_search_dir(D_CIT start, D_CIT end, const ci_string& dirName);//should return optional

	F_IT internal_search_file_or_link(F_IT start, F_IT end, const ci_string& fileName);

	bool hard_link_to_file_exists(const ci_string& fileName = "");//blank means any hardlink, special behavior for move operation
	bool soft_link_to_file_exists(const ci_string& fileName);

public:
	tDir(tDir* parent, const ci_string& name);
	tDir(tDir&&) = default;
	tDir(const tDir&);
	tDir& operator=(tDir&&) = default;
	tDir& operator=(tDir&);
	~tDir() = default;

	ci_string path();
	ci_string name();
	bool empty();
	bool can_be_removed();

	tDir* parent();
	tDir* get_dir_by_name(const ci_string& dirName);//add version
	std::shared_ptr<tFileBase> get_file_or_link_by_file(std::shared_ptr<tFileBase> file);
	std::shared_ptr<tFileBase> get_file_by_name(const ci_string& fileName);//add version
	std::vector<std::shared_ptr<tFileBase>> get_all_files_by_name(const ci_string& fileName);

	void create_dir(const ci_string& dirName);
	void remove_dir(const ci_string& dirName);
	void recursive_remove_dir(const ci_string& dirName);
	
	void create_file(const ci_string& fileName);
	void remove_file(const ci_string& fileName);//simply removes file (or link) from dir.
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

