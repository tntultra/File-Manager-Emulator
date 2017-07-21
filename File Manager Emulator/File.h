#pragma once
#include <string>
#include <memory>

class tFile
{
	struct tFileImpl;
	std::unique_ptr<tFileImpl> pImpl;
	public:
		explicit tFile(const std::string& url);
		bool get_command(std::string* s);
};

