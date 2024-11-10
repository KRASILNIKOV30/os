#pragma once
#include "Child.h"
#include "Request.h"
#include "SocketPair.h"
#include <fstream>
#include <bit>
#include <numeric>

class Child
{
public:
	explicit Child(Socket&& socket)
		: m_socket(std::move(socket))
	{
	}

	void Run()
	{
		while (true)
		{
			auto request = GetRequest();

			if (request.type == RequestType::EXIT)
			{
				return;
			}
			if (request.type == RequestType::ADD)
			{
				HandleAddRequest(request);
			}
			else if (request.type == RequestType::LONGEST_WORD)
			{
				HandleLongestWordRequest(request);
			}
		}
	}

private:
	void HandleAddRequest(Request const& request)
	{
		const auto bodyLength = request.bodyLength;
		int numbers[bodyLength];
		m_socket.Read(numbers, sizeof(numbers));
		const auto result = std::accumulate(numbers, numbers + bodyLength, 0);
		auto response = Response(
		{
			.sum = result,
		});
		const auto responseBytes = std::bit_cast<uint8_t*>(&response);
		m_socket.Write(responseBytes, sizeof(Response));
	}

	static std::string FindLongestWord(std::string const& fileName)
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

	std::string GetFileName(Request const& request)
	{
		const auto bodyLength = request.bodyLength;
		char buffer[bodyLength];
		m_socket.Read(buffer, bodyLength);
		return { buffer, bodyLength };
	}

	void HandleLongestWordRequest(Request const& request)
	{
		const auto fileName = GetFileName(request);
		const auto longestWord = FindLongestWord(fileName);
		auto response = Response(
		{
			.longestWordLength = longestWord.size(),
		});
		const auto responseBytes = std::bit_cast<uint8_t*>(&response);
		m_socket.Write(responseBytes, sizeof(Response));
		m_socket.Write(longestWord.c_str(), longestWord.size());
	}

	Request GetRequest()
	{
		uint8_t* readBytes[sizeof(Request)];
		m_socket.Read(readBytes, sizeof(Request));
		return *std::bit_cast<Request*>(&readBytes);
	}

	Socket m_socket;
};