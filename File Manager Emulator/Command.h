#pragma once
#include <memory>
#include <string>
#include <vector>
#include "FileManager.h"

struct tCommandImpl;

//abstract class, interface
class tCommand
{
	struct tPath
	{
		bool IsHardLink = false;
		bool IsSoftLink = false;
		std::vector<ci_string> Path;
	};

	virtual void executeImpl() = 0;
	void split_path(tPath* path, const ci_string& text);
protected:

	tFileManager* Receiver;
	ci_string Name;
	tPath Source;
	tPath Dest;

public:
	tCommand(tFileManager* receiver, std::vector<ci_string>& parsedStr);
	virtual ~tCommand() = default;

	void execute()
	{
		return executeImpl();
	}

};