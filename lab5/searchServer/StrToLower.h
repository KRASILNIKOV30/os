#pragma once
#include <string>

inline char ToLower(const char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

inline std::string StrToLower(std::string s)
{
	std::transform(s.begin(), s.end(), s.begin(),
		[](const unsigned char c){ return std::tolower(c); });
	return s;
}