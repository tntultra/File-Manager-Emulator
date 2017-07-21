#include "stdafx.h"
#include "FileManager.h"
#include <list>
#include "FileBase.h"

struct tFileManager::tFileManagerImpl
{
	char DriveLetter;
	std::list<tFileBase> Head;
};
