#pragma once
#include <cstddef>

enum class RequestType
{
	ADD,
	LONGEST_WORD,
	EXIT,
};

// request header
struct Request
{
	RequestType type;
	size_t bodyLength;
};

// header и body
struct Response
{
	int sum;
	size_t longestWordLength;
};