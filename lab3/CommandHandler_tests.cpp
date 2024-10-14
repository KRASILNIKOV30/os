#include "ChildProcessManager.h"
#include "Request.h"
#include "CommandHandler.h"
#include "CommandHandler.cpp"
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
		auto pipedCommandHandler = PipedChildProcessManager(RunCommandHandler);
		const Fetcher fetcher(pipedCommandHandler);

		THEN("can fetch sum from one argument")
		{
			CHECK(fetcher.FetchSumCalculation({ 5 }) == 5);
		}

		AND_THEN("can fetch sum from many arguments")
		{
			CHECK(fetcher.FetchSumCalculation({ 1, 2, 3, -4 }) == 2);
		}

		AND_THEN("can fetch sum from zero arguments")
		{
			CHECK(fetcher.FetchSumCalculation({}) == 0);
		}
	}
}
