#include <ostream>
#include <random>

inline void Generate(std::ostream& output, size_t size)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, std::numeric_limits<uint64_t>::max());

	for (size_t i = 0; i < size; i++)
	{
		output << dist(rng) << std::endl;
	}
}


