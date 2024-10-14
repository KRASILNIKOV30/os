#pragma once
#include "ChildProcessManager.h"
#include <span>

class Fetcher
{
public:
	explicit Fetcher(PipedChildProcessManager& childProcessManager)
		: childProcessManager(childProcessManager)
	{
	}

	[[nodiscard]] int FetchSumCalculation(std::vector<int> const& args) const
	{
		SendSumCalculation(args);
		return ReceiveSumCalculation();
	}

private:
	void SendSumCalculation(std::vector<int> const& args) const
	{
		SendSumCalculationHeader(args);
		SendSumCalculationBody(args);
	}

	void SendSumCalculationHeader(std::vector<int> const& args) const
	{
		auto addCommand = Request({
			.type = RequestType::ADD,
			.bodyLength = args.size(),
		});
		const auto addCommandBytes = std::bit_cast<uint8_t*>(&addCommand);
		childProcessManager.SendToChild(addCommandBytes, sizeof(Request));
	}

	void SendSumCalculationBody(std::vector<int> const& args) const
	{
		const auto body = std::as_bytes(std::span(args));
		childProcessManager.SendToChild(body.data(), body.size());
	}

	[[nodiscard]] int ReceiveSumCalculation() const
	{
		uint8_t* responseBuffer[sizeof(Response)];
		childProcessManager.ReceiveFromChild(responseBuffer, sizeof(responseBuffer));
		const auto response = std::bit_cast<Response*>(&responseBuffer);
		return response->sum;
	}

	PipedChildProcessManager& childProcessManager;
};
