#pragma once
#include "ProcessManager.h"
#include <span>
#include <vector>

class Fetcher
{
public:
	explicit Fetcher(ProcessManager&& childProcessManager)
		: childProcessManager(childProcessManager)
	{
	}

	[[nodiscard]] int FetchSumCalculation(std::vector<int> const& args) const
	{
		SendRequest({
			.type = RequestType::ADD,
			.bodyLength = args.size(),
		});
		SendData(args);
		return GetResponse().sum;
	}

	[[nodiscard]] std::string FetchLongestWordSearching(std::string const& fileName) const
	{
		SendRequest({
			.type = RequestType::LONGEST_WORD,
			.bodyLength = fileName.size(),
		});
		SendData(fileName);
		auto size = GetResponse().longestWordLength;
		char buffer[size];
		childProcessManager.ReceiveFromChild(buffer, size);
		return { buffer, size };
	}

private:
	void SendRequest(Request const& request) const
	{
		const auto bytes = std::bit_cast<uint8_t*>(&request);
		childProcessManager.SendToChild(bytes, sizeof(Request));
	}

	template<typename T>
	void SendData(T const& data) const
	{
		const auto dataBytes = std::as_bytes(std::span(data));
		childProcessManager.SendToChild(dataBytes.data(), dataBytes.size());
	}

	[[nodiscard]] Response GetResponse() const
	{
		uint8_t responseBuffer[sizeof(Response)];
		childProcessManager.ReceiveFromChild(responseBuffer, sizeof(Response));
		return *std::bit_cast<Response*>(&responseBuffer);
	}

	// переименовать
	ProcessManager& childProcessManager;
};
