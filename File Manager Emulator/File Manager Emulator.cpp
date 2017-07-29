// File Manager Emulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "File.h"
#include "CommandFactory.h"
#include <fstream>
#include "FileManagerController.h"

int main()
{
	/*tFile batch{ "some_batch_file.bat" };
	std::string command;
	tFileManagerController Controller;
	while (batch.get_command (&command)) {
		Controller.process_command(tCommandFactory::create_command(command));
	}
	Controller.display_current_files();*/

	std::ifstream batch{ "E:\\fme_batch.bat" };
	tFileManagerController Controller;
	Controller.process_stream(&batch);

	return 0;
}

