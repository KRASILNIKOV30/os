#include "Path.h"
#include <string>
#include <unordered_set>

struct Document
{
	uint64_t id;
	Path path;
	std::unordered_set<std::string> words{};
};
