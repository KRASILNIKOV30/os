#include "CommandHandler.h"
#include "Request.h"
#include "catch2/internal/catch_console_colour.hpp"
#include <fstream>
#include <bit>
#include <numeric>

inline void HandleAddRequest(FileDesc& input, FileDesc& output, Request const& request)
{
	const auto bodyLength = request.bodyLength;
	int numbers[bodyLength];
	input.Read(numbers, sizeof(numbers));
	const auto result = std::accumulate(numbers, numbers + bodyLength, 0);
	auto response = Response(
	{
		.sum = result,
	});
	const auto responseBytes = std::bit_cast<uint8_t*>(&response);
	output.Write(responseBytes, sizeof(Response));
}

inline std::string FindLongestWord(std::string const& fileName)
{
	std::ifstream file(fileName);
	std::string word;
	std::string longestWord;
	while (file >> word)
	{
		if (word.size() > longestWord.size())
		{
			longestWord = word;
		}
	}

	return longestWord;
}

inline std::string GetFileName(FileDesc& input, Request const& request)
{
	const auto bodyLength = request.bodyLength;
	char buffer[bodyLength];
	input.Read(buffer, bodyLength);
	return { buffer, bodyLength };
}

inline void HandleLongestWordRequest(FileDesc& input, FileDesc& output, Request const& request)
{
	auto fileName = GetFileName(input, request);
	auto longestWord = FindLongestWord(fileName);
	auto response = Response(
	{
		.longestWordLength = longestWord.size(),
	});
	const auto responseBytes = std::bit_cast<uint8_t*>(&response);
	output.Write(responseBytes, sizeof(Response));
	output.Write(longestWord.c_str(), longestWord.size());
}

inline Request GetRequest(FileDesc& input)
{
	uint8_t* readBytes[sizeof(Request)];
	input.Read(readBytes, sizeof(Request));
	return *std::bit_cast<Request*>(&readBytes);
}

inline void RunCommandHandler(FileDesc&& input, FileDesc&& output)
{
	while (true)
	{
		auto request = GetRequest(input);

		if (request.type == RequestType::EXIT)
		{
			return;
		}
		if (request.type == RequestType::ADD)
		{
			HandleAddRequest(input, output, request);
		}
		else if (request.type == RequestType::LONGEST_WORD)
		{
			HandleLongestWordRequest(input, output, request);
		}
	}
}