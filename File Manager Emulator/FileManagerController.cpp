#include "stdafx.h"
#include "FileManagerController.h"
#include "CommandFactory.h"
#include <iostream>
#include "CIString.h"

tCommandFactory tFileManagerController::Factory;


tFileManagerController :: tFileManagerController() :
	FileManager{ std::make_unique<tFileManager>() }
{}


void tFileManagerController::process_stream(std::istream* stream)
{
	ci_string newCommandText;
	while (getline (*stream, newCommandText)) {
		if (newCommandText.empty()) {
			continue;
		}
		try {
			auto newCommand = tFileManagerController::Factory.create_command(FileManager.get(), newCommandText);
			newCommand->execute();
		} catch (const std::runtime_error& rte) {
			std::cout << rte.what() << std::endl;
		}
	}
}

void tFileManagerController::display_current_tree(std::ostream* stream)
{
	//FileManager->print(stream);
}

