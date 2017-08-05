#include "stdafx.h"
#include "FileManager.h"
#include "FileBase.h"
#include <queue>
#include <unordered_set>
#include <iostream>

INode* tFileManager::create_new_node (INode::INodeType)
{
	INode newNode;
	newNode.Id = ++LastNodeIndex;
	Nodes.emplace_back(std::move(newNode));
	return &Nodes.back();
}

INode* tFileManager::node_by_id (unsigned id)
{
	return NodeIndex[id];
}

unsigned tFileManager::get_dir_by_path(const std::vector<ci_string>& path, bool parent)
{
	if (!path.empty()) {
		auto pathHasFullName = tome(path.front());
		auto startDir = pathHasFullName ? &*Nodes.begin() : CurrentDir;
		auto end = has_extension(path) || parent ? path.end() - 1 : path.end();
		for (auto it = pathHasFullName ? path.begin()+1 : path.begin (); startDir && it != end; ++it) {
			startDir = NodeIndex[startDir->DataBlock->get_node_by_name(*it)];
		}
		return startDir-> Id;
	}
	return UNDEFINED_INODE_ID;
}

/*bool tFileManager::path_represents_current_dir(const std::vector<ci_string>& path)
{
	return CurrentDir-> path () == path;
}*/

unsigned tFileManager::get_file_by_path(const std::vector<ci_string>& path)
{
	if (!has_extension(path))
		return UNDEFINED_INODE_ID;
	auto fileDir = get_dir_by_path(path, false);
	if (!fileDir)
		return UNDEFINED_INODE_ID;
	return node_by_id(fileDir)->DataBlock->get_node_by_name(get_name(path));
}

/*bool tFileManager::dir_can_be_removed (unsigned dirId)
{
	std::queue<INode*> dirs;
	dirs.push(NodeIndex[dirId]);
	while (!dirs.empty()) {
		auto dirNode = dirs.front();
		dirs.pop();
		if (dir_has_hardlinks_to_files (dirNode)) {
			return false;
		}
		for (auto& childDir : dirNode->DataBlock->Refs) {
			dirs.push(NodeIndex[childDir.first]);
		}
	}
}*/

INode* tFileManager::copy_node (unsigned nodeId)
{
	auto nodeToCopy = NodeIndex[nodeId];
	switch (nodeToCopy->Type) {
	case INode::INodeType::DIR: {
		auto newDir = create_new_node(INode::INodeType::DIR);
		for (auto&& node : nodeToCopy->DataBlock->Refs) {
			if (node.second == "." || node.second == "..") {
				continue;
			}
			if (node.second.find("hlink") == std::string::npos) {
				auto newNode = copy_node(node.first);
				newDir->DataBlock->Refs.insert(std::make_pair(newNode->Id, node.second));
			}
			else {
				newDir->DataBlock->Refs.insert(node);
				++NodeIndex[node.first]->HardRefCount;
			}
		}
		return newDir;
	}
	case INode::INodeType::FILE: {
		return create_new_node(INode::INodeType::FILE);
	}
	case INode::INodeType::SOFTLINK: {
		auto newSoftLink = create_new_node(INode::INodeType::SOFTLINK);
		++NodeIndex[nodeToCopy->DataBlock->Refs.begin()->first]->SoftRefCount;
		newSoftLink->DataBlock->Refs = nodeToCopy->DataBlock->Refs;
		return newSoftLink;
	}
	default:
		break;
	}
	return nullptr;
}

//MF – creates a file.
//Command format : MF[drive:]path
//Notes : If such file already exists with the given path then FME should continue to the next
//command in the batch file without any error rising.
void tFileManager::create_file(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	auto fileName = get_name(path);
	if (dir!= UNDEFINED_INODE_ID) {
		auto file = NodeIndex[dir]->DataBlock->get_node_by_name(fileName);
		if (!file) {
			auto newFile = create_new_node(INode::INodeType::FILE);
			node_by_id(dir)->DataBlock->Refs[newFile->Id] = fileName;
		}
	}
}

//MD – creates a directory.
//Command format : MD[drive:]path
//Notes : MD should not create any intermediate directories in the path.
void tFileManager::create_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, true);
	auto dirName = get_name(path);
	if (dir != UNDEFINED_INODE_ID) {
		auto newDir = create_new_node(INode::INodeType::DIR);
		node_by_id(dir)->DataBlock->Refs[newDir->Id] = dirName;
	}
}

//CD – changes the current directory.
//Command format : CD[drive:][path]
//Note that using CD without parameters is not allowed.
void tFileManager::change_dir(const std::vector<ci_string>& path)
{
	auto dir = get_dir_by_path(path, false);
	if (dir != UNDEFINED_INODE_ID) {
		 CurrentDir = NodeIndex[dir];
	}
}

//RD – removes a directory if it is empty(doesn’t contain any files or subdirectories).
//Command format : RD[drive:]path
//Notes : It is not allowed to delete the current directory in such way.
void tFileManager::remove_dir(const std::vector<ci_string>& path)
{
	auto dirId = get_dir_by_path(path, false);
	if (dirId != UNDEFINED_INODE_ID) {
		auto dirNode = NodeIndex[dirId];
		if (dirNode == CurrentDir) {
			return;
		}
		auto parentId = dirNode->DataBlock->get_node_by_name("..");
		if (parentId != UNDEFINED_INODE_ID) {
			auto refs = NodeIndex[parentId]->DataBlock->Refs;
			auto name = get_name(path);
			refs.erase(
				std::find_if(refs.begin(), refs.end(), [&refs,&name](auto&& pair) {
				return pair.second == name;
			}));
			NodeIndex.erase(dirId);
		}
	}
}

//DELTREE – removes a directory with all its subdirectories.
//Command format : DELTREE[drive:]path
//Note that you can’t remove a directory that contains current directory as one of its subdirectories.
//One can not delete a file which has an attached hard link but can delete a dynamic link.
//When a file is deleted its all dynamic links are also should be deleted.If a file has both hard and
//dynamic links FME should keep them all unchanged.
void tFileManager::recursive_remove_dir(const std::vector<ci_string>& path)
{
	auto dirId = get_dir_by_path(path, false);
	if (dirId == UNDEFINED_INODE_ID || dirId == CurrentDir-> Id)
		return;
	auto parentId = NodeIndex[dirId]->DataBlock->get_node_by_name ("..");
	if (parentId != UNDEFINED_INODE_ID) {

		//store all INode ids in this folder and then remove them all at once from Nodes
		std::unordered_set<unsigned> allInternalNodeIds;

		std::queue<INode*> allInternalNodes;
		allInternalNodes.push(NodeIndex[dirId]);
		while (!allInternalNodes.empty()) {
			auto node = allInternalNodes.front();
			allInternalNodes.pop();
			allInternalNodeIds.insert(node->Id);
			if (node->Type == INode::INodeType::DIR) {
				for (auto&& idNamePair : node->DataBlock->Refs) {
					if (idNamePair.first == CurrentDir-> Id) {
						return;//cannot remove dir with current dir in it
					}
					auto name = idNamePair.second;
					if (name.find ("hlink") == std::string::npos) {
						return;//cant remove dir with hlink in it
					}
					if (name != "." || name != "..") {//skip parent and current refs
						allInternalNodes.push(NodeIndex[idNamePair.first]);
					}
				}
			}
		}

		//remove from parent if dir can be removed
		auto parent = NodeIndex[parentId];
		parent->DataBlock->Refs.erase(parent->DataBlock->Refs.find(dirId));

		//remove from Nodes and from NodeIndex

		/*auto removeFromNodeIndex = [this, &allInternalNodeIds](auto&& pair) {
			auto node = pair.second;
			if (node->Type != INode::INodeType::SOFTLINK) {
				return allInternalNodeIds.find(pair.first->Id) != allInternalNodeIds.end;
			}
			else {
				auto fileId = node->DataBlock.Refs.begin()-> first;//because softlink's datablock always had only 1 entry for file
				return allInternalNodeIds.find(fileId) != allInternalNodeIds.end;
			}
		};
		NodeIndex.erase(std::remove_if(NodeIndex.begin(), NodeIndex.end(), removeFromNodeIndex), NodeIndex.end());*/

		auto removeFromNodes = [this, &allInternalNodeIds](auto&& nodeRef) {
			if (nodeRef.Type != INode::INodeType::SOFTLINK) {
				return allInternalNodeIds.find(nodeRef.Id) != allInternalNodeIds.end();
			}
			else {
				auto fileId = nodeRef.DataBlock->Refs.begin()->first;//because softlink's datablock always had only 1 entry for file
				return allInternalNodeIds.find(fileId) != allInternalNodeIds.end();
			}
		};
		//Nodes.erase(std::remove_if(Nodes.begin(), Nodes.end(), removeFromNodes), Nodes.end());

		for (auto it = Nodes.begin(); it != Nodes.end();) {
			if (removeFromNodes(*it)) {
				NodeIndex.erase(it->Id);
				it = Nodes.erase(it);
			} else {
				++it;
			}
		}
	}
}
//MHL – creates a hard link to a file / directory and places it in given location.
//Command format : MHL[drive:]source[drive:]destination
//Notes : Destination should contain only path without any file name. 
//If such link already exists then FME should continue to the next
//	command in the batch file without any error rising.
//The output format for dynamic and hard links should be the following :
//hlink[full path] for hard links and dlink[full path] respectively.
void tFileManager::create_hard_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	if (has_extension(dest)) {
		return;
	}
	auto dirToInsertIntoId = get_dir_by_path(dest, false);
	if (dirToInsertIntoId == UNDEFINED_INODE_ID) {
		return;//not valid name of dest
	}
	auto sourceId = has_extension(source) ? get_file_by_path(source) : get_dir_by_path(source, false);
	if (sourceId != UNDEFINED_INODE_ID) {
		return;//not valid name of source
	}

	auto destNode = NodeIndex[dirToInsertIntoId];//guaranteed to be a directory
	auto hlinkName = "hlink[" + get_name(source) + "]";//name for a link in directory's datablock
	if (destNode->DataBlock->get_node_by_name(hlinkName) != UNDEFINED_INODE_ID) {
		return;//hlink already exists
	}

	++NodeIndex[sourceId]->HardRefCount;//increase ref count
	destNode->DataBlock->Refs[sourceId] = hlinkName;//add data to dest's node datablock
}

//MDL– creates a dynamic link to a file / directory and places it in given location.
//Command format : MDL[drive:]source[drive:]destination
//Notes : Destination should contain only path without any file name.
//If such link already exists then FME should continue to the next
//	command in the batch file without any error rising.
//The output format for dynamic and hard links should be the following :
//hlink[full path] for hard links and dlink[full path] respectively.
void tFileManager::create_soft_link(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	if (has_extension(dest)) {
		return;
	}
	auto dirToInsertIntoId = get_dir_by_path(dest, false);
	if (dirToInsertIntoId == UNDEFINED_INODE_ID) {
		return;//not valid name of dest
	}
	auto sourceId = has_extension(source) ? get_file_by_path(source) : get_dir_by_path(source, false);
	if (sourceId != UNDEFINED_INODE_ID) {
		return;//not valid name of source
	}

	//find in dest's datablock exactly following: softlink node with only ref id equal to sourceId
	auto destNode = NodeIndex[dirToInsertIntoId];//guaranteed to be a directory
	auto softlinkFindIt = std::find_if (destNode-> DataBlock->Refs.begin(), destNode->DataBlock->Refs.end(), [this,sourceId](auto&& idNamePair) {
		auto node = NodeIndex[idNamePair.first];
		return (node->Type == INode::INodeType::SOFTLINK && (*node->DataBlock->Refs.begin()).first == sourceId);
	});
	if (softlinkFindIt != destNode-> DataBlock->Refs.end()) {
		return;//softlink already exists
	}

	++NodeIndex[sourceId]->SoftRefCount;
	auto newNode = create_new_node(INode::INodeType::SOFTLINK);
	newNode->DataBlock->Refs.insert(std::make_pair (sourceId,"dlink[" + get_name(source) + "]"));
}

void tFileManager::remove_all_file_soft_links (unsigned fileId)
{
	std::unordered_set<unsigned> softLinksIdsToRemove;
	for (auto it = Nodes.begin(); it != Nodes.end();) {
		if (it-> Type == INode::INodeType::SOFTLINK) {
			if (it-> DataBlock-> Refs.begin()-> first == fileId) {
				softLinksIdsToRemove.insert(it->Id);
				NodeIndex.erase(it->Id);
				it = Nodes.erase(it);
			} else {
				++it;
			}
		}
	}
	//removed all softlinks, havent removed them from directories yet
	for (auto&& node : Nodes) {
		if (node.Type == INode::INodeType::DIR) {
			for (auto it = node.DataBlock->Refs.begin(); it != node.DataBlock->Refs.end();)	{
				if (softLinksIdsToRemove.find(it->first) != softLinksIdsToRemove.end()) {
					it = node.DataBlock->Refs.erase(it); // previously this was something like m_map.erase(it++);
				}
				else {
					++it;
				}
			}
		}
	}
}

//DEL – removes a file or link.
//Command format : DEL[drive:]path
//One can not delete a file which has an attached hard link but can delete a dynamic link.
//When a file is deleted its all dynamic links are also should be deleted.If a file has both hard and
//dynamic links FME should keep them all unchanged.
void tFileManager::delete_file_or_link(const std::vector<ci_string>& path)
{
	auto fileId = get_file_by_path(path);
	if (fileId == UNDEFINED_INODE_ID) {
		return;//no such file
	}
	auto fileNode = NodeIndex[fileId];
	if (fileNode->HardRefCount > 0) {
		return;//cannot delete file with hardlinks to it
	}
	if (fileNode->SoftRefCount > 0) {
		remove_all_file_soft_links(fileId);
	}
	NodeIndex[get_dir_by_path(path, true)]-> DataBlock-> Refs.erase(fileId);//remove record from dir's Refs
	NodeIndex.erase(fileId);//erase from index
	Nodes.erase (std::find_if (Nodes.begin (),Nodes.end (),[fileId](auto&& node) {
		return node.Id == fileId;
	}));//erase file node
}

//MOVE – move an existing directory / file / link to another location
//Command format : MOVE[drive:]source[drive:]destination
//Notes : Program should move directory with all its content.In case when a file or directory which is
//	being moved has a hard link, FME should terminate the MOVE operation and batch file execution.
//	In case when any dynamic link(s) found and no hard link exists, then dynamic link(s) should be
//	modified and contain new location information instead of the old one.
void tFileManager::move(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto sourceId = has_extension(source) ? get_file_by_path (source) : get_dir_by_path (source, false);
	auto sourceParentId = get_dir_by_path(source, true);
	auto destId = get_dir_by_path(dest, false);
	if (sourceId == UNDEFINED_INODE_ID || sourceParentId == UNDEFINED_INODE_ID || destId == UNDEFINED_INODE_ID) {
		return;
	}
	auto idNamePairIt = NodeIndex[sourceParentId]->DataBlock->Refs.find(sourceId);
	auto fileName = idNamePairIt->second;
	NodeIndex[sourceParentId]->DataBlock->Refs.erase(idNamePairIt);
	NodeIndex[destId]->DataBlock->Refs[sourceId] = fileName;
}

//COPY – copy an existed directory / file / link to another location.
//Command format : COPY[drive:]source[drive:]destination
//Notes : Program should copy directory with all its content.Destination path should not contain any
//	file name otherwise FME should raise error.
void tFileManager::copy(const std::vector<ci_string>& source, const std::vector<ci_string>& dest)
{
	auto sourceId = has_extension(source) ? get_file_by_path(source) : get_dir_by_path(source, false);
	auto sourceParentId = get_dir_by_path(source, true);
	auto destId = get_dir_by_path(dest, false);
	if (sourceId == UNDEFINED_INODE_ID || sourceParentId == UNDEFINED_INODE_ID || destId == UNDEFINED_INODE_ID || destId == sourceId) {
		return;
	}
	auto destNode = NodeIndex[destId];
	for (auto&& nodeInDestDir : destNode->DataBlock->Refs) {
		if (NodeIndex[nodeInDestDir.first]->Type != INode::INodeType::DIR) {
			std::cout << "Destination directory contains files!\n";
		}
	}
	auto newNode = copy_node(sourceId);
	NodeIndex[destId]->DataBlock->Refs.insert(std::make_pair(newNode->Id, NodeIndex[sourceParentId]->DataBlock->Refs[sourceId]));
}

ci_string get_name(const std::vector<ci_string>& path)
{
	if (path.empty()) {
		return ci_string{};
	}
	return path.back();
}

bool tome(const ci_string& firstDir)
{
	return firstDir.find(':') != ci_string::npos;
}

bool has_extension (const std::vector<ci_string>& path)
{
	return get_name(path).rfind('.') != ci_string::npos;
}
