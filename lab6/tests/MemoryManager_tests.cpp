#define CATCH_CONFIG_MAIN
#include "../memoryManager/MemoryManager.h"
#include "catch.hpp"
#include "ThreadPool.h"
#include <thread>

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
				const auto value = std::construct_at(static_cast<double*>(ptr), 3.1415927);
				CHECK(*value == 3.1415927);
			}

			memoryManager.Free(ptr);
		}
	}
}

TEST_CASE("initializing with not aligned memory")
{
	alignas(sizeof(std::max_align_t)) uint8_t buffer[32];
	CHECK_THROWS_AS(MemoryManager(buffer + 1, sizeof(buffer)), std::invalid_argument);
}

SCENARIO("multiple blocks allocating")
{
	GIVEN("memory manager initialized with 128 bytes")
	{
		alignas(sizeof(max_align_t)) uint8_t buffer[96];
		auto const start = static_cast<uint8_t*>(buffer);
		MemoryManager memoryManager(buffer, sizeof(buffer));

		WHEN("allocate memory")
		{
			auto ptr1 = memoryManager.Allocate(16);

			THEN("memory allocated in the start of block after block header")
			{
				CHECK(ptr1 == start + 16);
			}

			AND_WHEN("allocate more blocks")
			{
				auto ptr2 = memoryManager.Allocate(16);
				auto ptr3 = memoryManager.Allocate(16);

				THEN("blocks allocated")
				{
					CHECK(ptr2 == start + 48);
					CHECK(ptr3 == start + 80);

					AND_THEN("all memory was allocated")
					{
						const auto ptr4 = memoryManager.Allocate(1);
						CHECK(ptr4 == nullptr);
					}

					WHEN("free the first block")
					{
						memoryManager.Free(ptr1);
						ptr1 = nullptr;

						THEN("can allocate it again")
						{
							ptr1 = memoryManager.Allocate(16);
							CHECK(ptr1 == start + 16);
						}
					}

					WHEN("free the second block")
					{
						memoryManager.Free(ptr2);
						ptr2 = nullptr;

						THEN("can allocate it again")
						{
							ptr2 = memoryManager.Allocate(16);
							CHECK(ptr2 == start + 48);
						}
					}

					WHEN("free the third block")
					{
						memoryManager.Free(ptr3);
						ptr3 = nullptr;

						THEN("can allocate it again")
						{
							ptr3 = memoryManager.Allocate(16);
							CHECK(ptr3 == start + 80);
						}
					}
				}

				memoryManager.Free(ptr2);
				memoryManager.Free(ptr3);
			}

			memoryManager.Free(ptr1);
		}

		WHEN("allocate memory with unaligned size")
		{
			const auto ptr1 = memoryManager.Allocate(17);

			WHEN("allocate memory")
			{
				const auto ptr2 = memoryManager.Allocate(16);

				THEN("manager aligned memory")
				{
					CHECK(ptr1 == start + 16);
					CHECK(ptr2 == start + 64);
				}

				AND_THEN("can not allocate more memory")
				{
					const auto ptr3 = memoryManager.Allocate(1);
					CHECK(ptr3 == nullptr);
				}

				memoryManager.Free(ptr2);
			}

			WHEN("allocate with custom alignment")
			{
				const auto ptr2 = memoryManager.Allocate(16, 8);

				THEN("memory allocated")
				{
					CHECK(ptr2 == start + 56);
				}

				memoryManager.Free(ptr2);
			}

			memoryManager.Free(ptr1);
		}

	}
}

SCENARIO("Allocate and free memory blocks")
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
				REQUIRE(ptr == ptrAfterFree); // Ensures that memory was reused correctly
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

SCENARIO("treadsafe")
{
	GIVEN("A memory manager initialized with 128B of memory")
	{
		char buffer[1024 * 1024];
		MemoryManager manager(buffer, sizeof(buffer));
		ThreadPool pool(12);

		WHEN("Allocate memory from different threads")
		{
			for (int i = 0; i < 1000; i++)
			{
				pool.Dispatch([&]() { manager.Allocate(16, 0); });
			}
			pool.Wait();

			THEN("memory allocated")
			{
				CHECK(true);
			}
		}
	}
}

SCENARIO("multiple freed blocks")
{
	GIVEN("A memory manager initialized with 128B of memory")
	{
		char buffer[128];
		MemoryManager manager(buffer, sizeof(buffer));

		WHEN("allocate and free some memory")
		{
			auto const ptr1 = manager.Allocate(16);
			manager.Free(ptr1);

			THEN("can allocate larger block of memory")
			{
				auto const ptr2 = manager.Allocate(17);
				CHECK(ptr2 == buffer + 48);
			}
		}
	}
}
