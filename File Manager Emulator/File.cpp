#include "stdafx.h"
#include "File.h"
#include <fstream>

struct tFile :: tFileImpl
{
	explicit tFileImpl(const std::string& url) :
		stream {url}
	{}

	std::ifstream stream;
};

tFile::tFile (const std::string& url) :
	pImpl{ new tFileImpl{url} }
{
	
}

bool tFile::get_command (std::string* s)
{
	*s = "";
	if (!pImpl->stream.eof()) {
		std::getline(pImpl->stream, *s);
		return true;
	}
	return false;
}
