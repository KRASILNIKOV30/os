#include "TreadsafeSet.h"
#include <iostream>
#include <cmath>
#include <optional>
#include <thread>

struct Args
{
	uint64_t limit;
	int threadsNum;
};

std::optional<Args> ParseArgs(const int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cout << "Usage: " << argv[0] << " <limit> <threadsNum>" << std::endl;
		return std::nullopt;
	}

	Args args{};
	try
	{
		args.limit = std::stoi(argv[1]);
		args.threadsNum = std::stoi(argv[2]);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return std::nullopt;
	}

	return args;
}

bool IsPrime(const uint64_t n)
{
	if (n <= 1)
	{
		return false;
	}
	if (n <= 3)
	{
		return true;
	}

	if (n % 2 == 0 || n % 3 == 0)
	{
		return false;
	}

	for (uint64_t i = 5; i * i <= n; i += 6)
	{
		if (n % i == 0 || n % (i + 2) == 0)
		{
			return false;
		}
	}
	return true;
}

void FindPrimes(ThreadsafeSet<uint64_t>& primes, const uint64_t from, const uint64_t to)
{
	for (uint64_t i = from; i <= to; ++i)
	{
		if (IsPrime(i))
		{
			primes.Insert(i);
		}
	}
}

int main(const int argc, char* argv[])
{
	const auto args = ParseArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	ThreadsafeSet<uint64_t> primes;

	const auto limit = args->limit;
	const auto threadsNum = args->threadsNum;

	std::vector<std::jthread> threads;
	uint64_t range = limit / threadsNum;

	for (unsigned int i = 0; i < threadsNum; ++i)
	{
		uint64_t start = i * range + 2;
		uint64_t end = (i == threadsNum - 1) ? limit : start + range - 1;
		threads.emplace_back(FindPrimes, std::ref(primes), start, end);
	}

	return EXIT_SUCCESS;
}