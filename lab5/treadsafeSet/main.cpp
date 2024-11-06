#include "TreadsafeSet.h"
#include "../../timer/Timer.h"
#include <iostream>
#include <cmath>
#include <optional>
#include <thread>
#include <memory>

struct Args
{
	uint64_t limit;
	int threadsNum;
};

std::optional<Args> ParseArgs(const int argc, char* argv[])
{
	if (argc != 3)
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

void FindPrimesInRange(const std::shared_ptr<ISet<uint64_t>>& primes, const uint64_t start, const uint64_t end)
{
	for (uint64_t i = start; i <= end; ++i)
	{
		if (IsPrime(i))
		{
			primes->Insert(i);
		}
	}
}

void FindPrimes(uint64_t const limit, int const threadNum, const std::shared_ptr<ISet<uint64_t>>& primes)
{

	std::vector<std::jthread> threads;
	const auto range = limit / threadNum;

	for (unsigned int i = 0; i < threadNum - 1; ++i)
	{
		uint64_t start = i * range + 2;
		uint64_t end = start + range - 1;
		threads.emplace_back(FindPrimesInRange, std::ref(primes), start, end);
	}
	FindPrimesInRange(primes, limit - range, limit);
}

int main(const int argc, char* argv[])
{
	const auto args = ParseArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	const auto limit = args->limit;
	const auto threadsNum = args->threadsNum;

	std::shared_ptr<ISet<uint64_t>> primesSet = std::make_shared<Set<uint64_t>>();
	std::shared_ptr<ISet<uint64_t>> threadsafePrimesSet = std::make_shared<ThreadsafeSet<uint64_t>>();

	for (int tNum = 1; tNum <= threadsNum; ++tNum)
	{
		auto& primes = tNum == 1
			? primesSet
			: threadsafePrimesSet;
		MeasureTime(std::cout, std::to_string(tNum) + " thread sorting", FindPrimes, limit, tNum, std::ref(primes));
	}

	std::cout << std::endl << "simple set primes:" << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << primesSet->Get(i) << std::endl;
	}

	std::cout << std::endl << "threadsafe set primes:" << std::endl;
	for (int i = 0; i < 10; ++i)
	{
		std::cout << threadsafePrimesSet->Get(i) << std::endl;
	}

	return EXIT_SUCCESS;
}