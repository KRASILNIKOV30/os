#pragma once

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
	size_t longestWordLength;
};