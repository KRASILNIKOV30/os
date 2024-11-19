#include "Path.h"
#include <string>
#include <unordered_map>

using Words = std::unordered_map<std::string, size_t>;

struct Document
{
	uint64_t id;
	Path path;
	Words words{};
	size_t wordCount;
};
