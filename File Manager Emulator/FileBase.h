#pragma once

#include "CIString.h"

#include <memory>
#include <unordered_map>

extern const unsigned UNDEFINED_INODE_ID;

//soft (dynamic) link contains only 1 entry: fileNodeId : linkName
//dir contains 2 default entries : ".", ".." and then entry for each file/dir in dir. file/dirNodeId : file/DirName
//for hardlinks dir contains entry with same Inode id, but name is decorated with hlink[]
struct DataBlock
{
	unsigned get_node_by_name(const ci_string& name);
	ci_string get_name_by_id(unsigned id);

	std::unordered_map<unsigned, ci_string> Refs;
	//std::unordered_map<ci_string, unsigned> StringRefs;
};

//hardlink adds additional entry in dir for a fileNodeId and increases it's HardRefCount


struct INode // THIS IS A FILE ITSELF
{

	enum class INodeType : int8_t
	{
		FILE,
		DIR,
		SOFTLINK,
		UNDEFINED
	};


	unsigned Id = static_cast<unsigned> (-1);
	unsigned HardRefCount = 0;
	unsigned SoftRefCount = 0;//for fast checking about softlink's existance on deletion
	INodeType Type = INodeType::UNDEFINED;//file,dir,softlink ("dynamic link")
	std::unique_ptr<DataBlock> DataBlock = nullptr;//file has no datablock
};
