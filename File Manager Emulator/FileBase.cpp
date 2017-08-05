#include "stdafx.h"
#include "FileBase.h"
#include <algorithm>

const unsigned UNDEFINED_INODE_ID = static_cast<unsigned>(-1);

unsigned DataBlock::get_node_by_name (const ci_string& name)
{
	auto it = std::find_if (Refs.begin(),Refs.end(),[&name](auto&& pair)
	{
		return pair.second == name;
	});
	if (it != Refs.end()) {
		return it->first;
	}
	return UNDEFINED_INODE_ID;
}
