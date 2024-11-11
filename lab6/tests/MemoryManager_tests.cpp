#define CATCH_CONFIG_MAIN
#include "../memoryManager/MemoryManager.h"
#include "catch.hpp"

SCENARIO("simple data constructing")
{
	GIVEN("memory manager initialized with 1000 bytes")
	{
		alignas(sizeof(max_align_t)) uint8_t buffer[1000];
		MemoryManager memoryManager(buffer, sizeof(buffer));

		WHEN("allocate memory")
		{
			auto ptr = memoryManager.Allocate(sizeof(double));

			THEN("can construct value in allocated memory")
			{
				auto value = std::construct_at(static_cast<double*>(ptr), 3.1415927);
				CHECK(*value == 3.1415927);
			}

			memoryManager.Free(ptr);
		}
	}
}

SCENARIO("Allocate and free memory blocks", "[memory]")
{
	GIVEN("A memory manager initialized with 1MB of memory")
	{
		alignas(sizeof(max_align_t)) char rawMemory[1024 * 1024]; // Массив 1 МБ, который будет передан менеджеру
		MemoryManager manager(rawMemory, sizeof(rawMemory));

		WHEN("We allocate a small block of memory (128 bytes)")
		{
			void* ptr = manager.Allocate(128);

			THEN("memory should be successfully allocated")
			{
				REQUIRE(ptr != nullptr);
			}
		}

		WHEN("allocate a large block of memory (1024 bytes)")
		{
			void* ptr = manager.Allocate(1024);

			THEN("The memory should be successfully allocated")
			{
				REQUIRE(ptr != nullptr);
			}
		}

		WHEN("We allocate memory with custom alignment (64 bytes)")
		{
			void* ptr = manager.Allocate(128, 64);

			THEN("The memory should be aligned to the requested boundary")
			{
				REQUIRE(ptr != nullptr);
				REQUIRE(reinterpret_cast<uintptr_t>(ptr) % 64 == 0);
			}
		}

		WHEN("We try to allocate more memory than available (e.g., 1MB)")
		{
			void* ptr1 = manager.Allocate(1024 * 1024);
			void* ptr2 = manager.Allocate(1);

			THEN("The first allocation should fail, but the second should succeed")
			{
				REQUIRE(ptr1 == nullptr);
				REQUIRE(ptr2 != nullptr);
			}
		}

		WHEN("allocate and free a memory block (128 bytes)")
		{
			void* ptr = manager.Allocate(128);
			manager.Free(ptr);
			void* ptrAfterFree = manager.Allocate(128); // Reallocate the same size

			THEN("The memory should be freed and available for reuse")
			{
				REQUIRE(ptr != ptrAfterFree); // Ensures that memory was reused correctly
				REQUIRE(ptrAfterFree != nullptr);
			}
		}
	}
}

SCENARIO("Handling invalid parameters for memory allocation")
{
	GIVEN("A memory manager initialized with 1MB of memory")
	{
		char rawMemory[1024 * 1024];
		MemoryManager manager(rawMemory, sizeof(rawMemory));

		WHEN("We try to allocate memory with zero size")
		{
			void* ptr = manager.Allocate(0);

			THEN("The allocation should fail")
			{
				REQUIRE(ptr == nullptr);
			}
		}

		WHEN("We try to allocate memory with invalid alignment (zero alignment)")
		{
			void* ptr = manager.Allocate(128, 0);

			THEN("The allocation should fail")
			{
				REQUIRE(ptr == nullptr);
			}
		}

		WHEN("We try to allocate memory with non-power-of-two alignment (10 bytes)")
		{
			void* ptr = manager.Allocate(128, 10);

			THEN("The allocation should fail")
			{
				REQUIRE(ptr == nullptr);
			}
		}
	}
}

SCENARIO("Memory alignment checks")
{
	GIVEN("A memory manager initialized with 1MB of memory")
	{
		char rawMemory[1024 * 1024];
		MemoryManager manager(rawMemory, sizeof(rawMemory));

		WHEN("We allocate a block of memory with 8-byte alignment")
		{
			void* ptr = manager.Allocate(128, 8);

			THEN("The memory should be aligned to 8 bytes")
			{
				REQUIRE(ptr != nullptr);
				REQUIRE(reinterpret_cast<uintptr_t>(ptr) % 8 == 0);
			}
		}

		WHEN("We allocate a block of memory with 16-byte alignment")
		{
			void* ptr = manager.Allocate(128, 16);

			THEN("The memory should be aligned to 16 bytes")
			{
				REQUIRE(ptr != nullptr);
				REQUIRE(reinterpret_cast<uintptr_t>(ptr) % 16 == 0);
			}
		}
	}
}

SCENARIO("Memory manager correctly handles multiple allocations and frees", "[memory][leak]")
{
	GIVEN("A memory manager initialized with 1MB of memory")
	{
		char rawMemory[1024 * 1024];
		MemoryManager manager(rawMemory, sizeof(rawMemory));

		WHEN("We allocate and free multiple blocks of memory")
		{
			void* ptr1 = manager.Allocate(128);
			void* ptr2 = manager.Allocate(256);
			void* ptr3 = manager.Allocate(512);

			THEN("The allocations should be successful")
			{
				REQUIRE(ptr1 != nullptr);
				REQUIRE(ptr2 != nullptr);
				REQUIRE(ptr3 != nullptr);
			}

			WHEN("We free the allocated blocks and reallocate them")
			{
				manager.Free(ptr1);
				manager.Free(ptr2);
				manager.Free(ptr3);

				void* ptrAfterFree1 = manager.Allocate(128);
				void* ptrAfterFree2 = manager.Allocate(256);

				THEN("The freed memory should be reused without memory leak")
				{
					REQUIRE(ptrAfterFree1 != nullptr);
					REQUIRE(ptrAfterFree2 != nullptr);
				}

				manager.Free(ptrAfterFree1);
				manager.Free(ptrAfterFree2);
			}
		}
	}
}
