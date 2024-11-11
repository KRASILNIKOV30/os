#include "../threadPool/ThreadPool.h"
#include <catch.hpp>
#include <chrono>
#include <iostream>
#include <memory>

TEST_CASE("dispatch and execute")
{
	std::atomic<int> counter(0);
	ThreadPool pool(1);

	pool.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		counter.fetch_add(1);
	});

	pool.Wait();
	REQUIRE(counter.load() == 1);
}

TEST_CASE("stop before dispatch")
{
	ThreadPool pool(4);
	pool.Stop();

	bool taskExecuted = false;
	pool.Dispatch([&]() { taskExecuted = true; });

	REQUIRE(taskExecuted == false);
}

TEST_CASE("wait until all tasks are done")
{
	std::atomic<int> counter(0);
	ThreadPool pool(4);

	pool.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		counter.fetch_add(1);
	});
	pool.Dispatch([&]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		counter.fetch_add(1);
	});

	pool.Wait();
	REQUIRE(counter.load() == 2);
}

TEST_CASE("multiple threads dispatch")
{
	std::atomic<int> counter(0);
	const int numTasks = 100;
	ThreadPool pool(4);

	for (int i = 0; i < numTasks; ++i)
	{
		pool.Dispatch([&]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			counter.fetch_add(1);
		});
	}

	pool.Wait();
	REQUIRE(counter.load() == numTasks);
}

TEST_CASE("stop after dispatch")
{
	ThreadPool pool(4);
	pool.Dispatch([]() { std::this_thread::sleep_for(std::chrono::milliseconds(100)); });

	pool.Stop();
	pool.Wait();

	REQUIRE(true);
}