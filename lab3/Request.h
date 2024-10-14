#pragma once
#include <string>
#include <vector>

enum class RequestType
{
	ADD,
	LONGEST_WORD,
	EXIT,
};

struct Request
{
	RequestType type;
	size_t bodyLength;
};

struct Response
{
	int sum;
	int longestWordLength;
};