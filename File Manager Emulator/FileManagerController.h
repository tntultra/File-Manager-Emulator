#pragma once

#include <iosfwd>
#include "FileManager.h"
#include "CommandFactory.h"

class tFileManagerController
{
	static tCommandFactory Factory;
	std::unique_ptr<tFileManager> FileManager;
	void process_command();
	public:
		tFileManagerController();

		tFileManagerController(const tFileManagerController&) = delete;
		tFileManagerController(tFileManagerController&&) = delete;
		tFileManagerController& operator=(const tFileManagerController&) = delete;
		tFileManagerController& operator=(tFileManagerController&&) = delete;
		~tFileManagerController() = default;

		void process_stream(std::istream* stream);
		void display_current_tree(std::ostream* stream);
};

