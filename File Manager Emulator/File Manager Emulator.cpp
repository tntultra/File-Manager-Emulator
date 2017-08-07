// File Manager Emulator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "File.h"
#include "CommandFactory.h"
#include <fstream>
#include "FileManagerController.h"

int main()
{
	std::ifstream batch{ "E:\\fme_batch.bat" };
	tFileManagerController Controller;
	Controller.process_stream(&batch);

	return 0;
}

