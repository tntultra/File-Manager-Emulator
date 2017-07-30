#pragma once
#include <cctype>
#include <string>
#include <iosfwd>

struct ci_char_traits : public std::char_traits<char> {
	static bool eq(char c1, char c2) {
		return std::toupper(c1) == std::toupper(c2);
	}
	static bool lt(char c1, char c2) {
		return std::toupper(c1) <  std::toupper(c2);
	}
	static int compare(const char* s1, const char* s2, size_t n) {
		while (n-- != 0) {
			if (std::toupper(*s1) < std::toupper(*s2)) return -1;
			if (std::toupper(*s1) > std::toupper(*s2)) return 1;
			++s1; ++s2;
		}
		return 0;
	}
	//cppreference (and Sutter's book) have wrong implementation with int n which gives warnings bcz char_traits::find requires size_t.
	//if you do n-- for size_t you are shooting yourself in the leg.
	//also you should return not empty string, but nullptr if you dont find char.
	static const char* find(const char* s, size_t n, char a) {
		while (*s && std::toupper(*s) != std::toupper(a)) {
			++s;
		}
		return *s ? s : nullptr;
	}
};

typedef std::basic_string<char, ci_char_traits> ci_string;

std::ostream& operator<<(std::ostream& os, const ci_string& str);
std::istream& operator>>(std::istream& is, ci_string& str);

std::istream& getline(std::istream&  is, ci_string& str, char delim);
std::istream& getline(std::istream&& is, ci_string& str, char delim);
std::istream& getline(std::istream&  is, ci_string& str);
std::istream& getline(std::istream&& is, ci_string& str);