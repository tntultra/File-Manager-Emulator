#pragma once

#include "FileTypes.h"
#include "CIString.h"

#include <memory>
#include <string>
#include <map>
#include <vector>

class tDir
{
	INode* Node;

	D_IT internal_search_dir(D_IT start, D_IT end, const ci_string& dirName);//should return optional

	F_IT internal_search_file_or_link(F_IT start, F_IT end, const ci_string& fileName);

	bool hard_link_to_file_exists(const ci_string& fileName = "");//blank means any hardlink, special behavior for move operation
	bool soft_link_to_file_exists(const ci_string& fileName);

	virtual ci_string get_name_impl() noexcept;
	virtual INode::INodeType get_type_impl() noexcept;
	virtual tDir* clone_impl() const;

public:
	tDir(tDir* parent, const ci_string& name);
	tDir(tDir&&) = default;
	tDir(const tDir&);
	tDir& operator=(tDir&&) = default;
	tDir& operator=(tDir&);
	~tDir() = default;

	tDir* clone() const
	{
		return clone_impl();
	}

	ci_string path();
	bool empty();
	bool can_be_removed();
	bool tree_contains_dir(tDir* dir);

	unsigned parent();
	unsigned get_dir_by_name(const ci_string& dirName);//add version
	tFileBase* get_file_or_link_by_file(tFileBase* file);
	tFileBase* get_file_by_name(const ci_string& fileName);//add version
	std::vector<tFileBase*> get_all_files_by_name(const ci_string& fileName);

	void create_dir(const ci_string& dirName);
	void remove_dir(const ci_string& dirName);
	void recursive_remove_dir(const ci_string& dirName);
	
	void create_file(const ci_string& fileName);
	void remove_file(const ci_string& fileName);//simply removes file (or link) from dir.
	//filemanager handles all dynamic links that are associated with it

	void create_hard_link(tFileBase* file);
	void create_soft_link(tFileBase* file);

	void insert_other_dir(tDir* dir);

	void copy_file(const tFileBase& file);//all of the following functions work for links also
	void move_file(tFileBase&& file);


	friend bool operator< (tDir&, tDir&);
};

//void find_dir_in_dir(tDir* dir, tDir* findDir);

