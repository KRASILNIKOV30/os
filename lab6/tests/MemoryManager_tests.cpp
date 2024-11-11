#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../memoryManager/MemoryManager.h"

SCENARIO("simple data constructing")
{
	GIVEN("memory manager initialized with 1000 bytes")
	{
		alignas(sizeof(max_align_t)) uint8_t buffer[1000];
		MemoryManager memoryManager(buffer, sizeof(buffer));
	}
}