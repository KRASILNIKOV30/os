#include "CommandHandler.h"
#include "Request.h"
#include <bit>
#include <iostream>
#include <numeric>
#include <bits/ranges_algobase.h>
#include <pstl/glue_numeric_defs.h>

inline void RunCommandHandler(FileDesc&& input, FileDesc&& output)
{
	if (fork() == 0)
	{
		while (true)
		{
			uint8_t* readBytes[sizeof(Request)];
			input.Read(readBytes, sizeof(Request));
			const auto request = std::bit_cast<Request*>(&readBytes);
			if (request->type == RequestType::EXIT)
			{
				return;
			}
			if (request->type == RequestType::ADD)
			{
				const auto bodyLength = request->bodyLength;
				int numbers[bodyLength];
				input.Read(numbers, sizeof(numbers));
				const auto result = std::accumulate(numbers, numbers + bodyLength, 0);
				auto response = Response(
				{
					.sum = result,
				});
				const auto responseBytes = std::bit_cast<uint8_t*>(&response);
				output.Write(responseBytes, sizeof(responseBytes));
			}
			else if (request->type == RequestType::LONGEST_WORD)
			{

			}
		}
	}
}