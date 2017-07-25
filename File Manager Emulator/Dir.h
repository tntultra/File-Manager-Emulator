#pragma once

#include "FileTypes.h"

#include <memory>
#include <string>
#include <map>

class tDir
{
	friend class tFileManager;
	using F_IT = std::list<std::shared_ptr<tFileBase>>::iterator;
	using D_IT = std::list<tDir>::iterator;

	tDir* Parent;
	std::string Name;
	std::list<tDir> Dirs;
	std::list<std::shared_ptr<tFileBase> > Files;

	D_IT internal_search_dir(const std::string& dirName);//should return optional
	F_IT internal_search_file_or_link(const std::string& fileName);
	bool hard_link_to_file_exists(const std::string& fileName);
	bool soft_link_to_file_exists(const std::string& fileName);

public:
	tDir(tDir* parent, const std::string& name);
	tDir(tDir&&) = default;
	tDir(const tDir&);
	tDir& operator=(tDir&&) = default;
	tDir& operator=(const tDir&);
	~tDir() = default;

	std::string path() const;
	std::string name() const;
	bool empty() const;
	bool can_be_removed() const;

	tDir* parent() const;
	tDir* get_dir_by_name(const std::string& dirName);//add const version
	std::shared_ptr<tFileBase> get_file_by_name(const std::string& fileName);//add const version

	void create_dir(const std::string& dirName);
	void remove_dir(const std::string& dirName);
	void recursive_remove_dir(const std::string& dirName);
	
	void create_file(const std::string& fileName);
	void remove_file(const std::string& fileName);

	void create_hard_link(std::shared_ptr<tFileBase>);
	void create_soft_link(std::shared_ptr<tFileBase>);

	void insert_other_dir(tDir&& dir);
	void insert_other_dir(const tDir& dir);

	void insert_file(std::shared_ptr<tFileBase> file);
	void copy_file(std::shared_ptr<tFileBase> file);
	void move_file(std::shared_ptr<tFileBase> file);


	friend bool operator< (const tDir&, const tDir&);
};

//void find_dir_in_dir(std::shared_ptr<tDir> dir, std::shared_ptr<tDir> findDir);

