#define CATCH_CONFIG_MAIN
#include "../bgThreadDispatcher/BgThreadDispatcher.h"
#include "catch.hpp"
#include <chrono>
#include <iostream>
#include <sstream>
#include <syncstream>
#include <thread>

TEST_CASE("Test BgThreadDispatcher with multiple tasks")
{
	BgThreadDispatcher dispatcher;

	bool task1Done = false, task2Done = false, task3Done = false;

	dispatcher.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		task1Done = true;
	});
	dispatcher.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		task2Done = true;
	});
	dispatcher.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		task3Done = true;
	});

	dispatcher.Wait();

	REQUIRE(task1Done);
	REQUIRE(task2Done);
	REQUIRE(task3Done);
}

TEST_CASE("Test BgThreadDispatcher with exception handling")
{
	BgThreadDispatcher dispatcher;

	bool exceptionCaught = false;

	dispatcher.Dispatch([&]() {
		throw std::runtime_error("Test exception");
	});

	dispatcher.Dispatch([&]() {
		exceptionCaught = true;
	});

	dispatcher.Wait();

	REQUIRE(exceptionCaught);
}

TEST_CASE("Test BgThreadDispatcher Stop method")
{
	BgThreadDispatcher dispatcher;

	bool taskExecuted = false;

	dispatcher.Dispatch([&]() {
		taskExecuted = true;
	});

	dispatcher.Stop();

	dispatcher.Dispatch([&]() {
		taskExecuted = false;
	});

	dispatcher.Wait();

	REQUIRE(taskExecuted);
}

TEST_CASE("Test BgThreadDispatcher multiple Stops")
{
	BgThreadDispatcher dispatcher;

	dispatcher.Stop();
	dispatcher.Stop();

	bool taskExecuted = false;
	dispatcher.Dispatch([&]() { taskExecuted = true; });

	dispatcher.Wait();

	REQUIRE_FALSE(taskExecuted);
}

TEST_CASE("print 1 and exclamation")
{
	using osync = std::osyncstream;
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Dispatch([&output] { osync(output) << "1"; });
	dispatcher.Wait();
	osync(output) << "!";

	std::string result = output.str();
	REQUIRE((result == "1!" || result == "!1"));
}

TEST_CASE("print exclamation and numbers")
{
	using osync = std::osyncstream;
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Dispatch([&output] { osync(output) << "1"; });
	dispatcher.Dispatch([&output] { osync(output) << "2"; });
	osync(output) << "!";
	dispatcher.Wait();
	dispatcher.Dispatch([&output] { osync(output) << "3"; });
	dispatcher.Wait();

	std::string result = output.str();
	REQUIRE((result == "!123" || result == "1!23" || result == "12!3"));
}

TEST_CASE("print exclamation and possibly 1")
{
	using osync = std::osyncstream;
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Dispatch([&output] { osync(output) << "1"; });
	osync(output) << "!";

	std::string result = output.str();
	REQUIRE((result == "!1" || result == "1!" || result == "!"));
}

TEST_CASE("probability print 1 exclamation")
{
	using osync = std::osyncstream;
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Dispatch([&output] { osync(output) << "1"; });
	osync(output) << "!";
	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::string result = output.str();
	REQUIRE((result == "!1" || result == "1!" || result == "!"));
}

TEST_CASE("print exclamation no tasks")
{
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Wait();
	output << "!";

	std::string result = output.str();
	REQUIRE(result == "!");
}

TEST_CASE("probability print 1 stop and exclamation")
{
	using osync = std::osyncstream;
	std::stringstream output;

	BgThreadDispatcher dispatcher;
	dispatcher.Dispatch([&output] { osync(output) << "1"; });
	std::this_thread::sleep_for(std::chrono::seconds(1));
	dispatcher.Stop();
	dispatcher.Dispatch([&output] { osync(output) << "2"; });
	dispatcher.Wait();
	output << "!";

	std::string result = output.str();
	REQUIRE((result == "1!" || result == "!"));
}
