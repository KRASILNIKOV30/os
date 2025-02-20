#include "ProcessManager.h"
#include "Child.h"
#include "Fetcher.h"
#include "catch2/internal/catch_compiler_capabilities.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_reusable_string_stream.hpp"
#include "catch2/internal/catch_section.hpp"
#include "catch2/internal/catch_test_registry.hpp"
#include <bit>
#include <csignal>
#include <span>

SCENARIO("fetch sum calculation")
{
	GIVEN("fetcher")
	{
		ProcessManager pipedCommandHandler{ SocketPair() };
		if (pipedCommandHandler.GetPid() == 0)
		{
			return;
		}
		const Fetcher fetcher(std::move(pipedCommandHandler));

		CHECK(fetcher.FetchSumCalculation({ 5 }) == 5);
		CHECK(fetcher.FetchSumCalculation({ 1, 2, 3, -4 }) == 2);
		CHECK(fetcher.FetchSumCalculation({}) == 0);

		CHECK(fetcher.FetchLongestWordSearching("in.txt") == "hello");
	}
}
