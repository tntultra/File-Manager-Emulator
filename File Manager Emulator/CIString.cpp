#include "stdafx.h"
#include "CIString.h"

std::ostream& operator<<(std::ostream& os, const ci_string& str) {
	return os.write(str.data(), str.size());
}

std::istream& operator>>(std::istream& is, ci_string& str) {
	std::string buffer;
	is >> buffer;
	str = buffer.c_str();
	return is;
}

std::istream& getline(std::istream&  is, ci_string& str, char delim)
{
	std::string buffer;
	std::getline(is, buffer, delim);
	str = buffer.c_str();
	return is;
}
std::istream& getline(std::istream&& is, ci_string& str, char delim)
{
	std::string buffer;
	std::getline(is, buffer, delim);
	str = buffer.c_str();
	return is;
}

std::istream& getline(std::istream&  is, ci_string& str)
{
	std::string buffer;
	std::getline(is, buffer);
	str = buffer.c_str();
	return is;
}

std::istream& getline(std::istream&& is, ci_string& str)
{
	std::string buffer;
	std::getline(is, buffer);
	str = buffer.c_str();
	return is;
}

size_t ci_string_hash(const ci_string& ciStr)
{
	return std::hash<std::string>()(ciStr.c_str());
}