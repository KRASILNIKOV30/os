#pragma once
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <mutex>

class MemoryManager
{
public:
	MemoryManager(void* start, size_t size)
	{
		if (reinterpret_cast<std::uintptr_t>(start) % alignof(std::max_align_t) != 0)
		{
			throw std::invalid_argument("Start address must be aligned to std::max_align_t");
		}

		m_freeList = static_cast<BlockHeader*>(start);
		m_freeList->size = size;
		m_freeList->next = nullptr;
	}

	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

	void* Allocate(const size_t size, const size_t align = alignof(std::max_align_t)) noexcept
	{
		std::lock_guard lock(m_mutex);
		if (size == 0 || align == 0 || (align & (align - 1)) != 0)
		{
			return nullptr;
		}

		BlockHeader** prev = &m_freeList;
		BlockHeader* curr = m_freeList;

		while (curr != nullptr)
		{
			constexpr auto headerSize = sizeof(BlockHeader);
			const auto addr = reinterpret_cast<ptrdiff_t>(curr) + headerSize;
			const size_t alignedAddr = (addr + align - 1) & ~(align - 1);

			const size_t offset = alignedAddr - reinterpret_cast<uintptr_t>(curr);

			if (curr->size >= offset + size)
			{
				if (curr->size > offset + size + sizeof(BlockHeader))
				{
					auto* newBlock = reinterpret_cast<BlockHeader*>(alignedAddr + size);
					newBlock->size = curr->size - offset - size;
					newBlock->next = curr->next;

					curr->size = offset;
					curr->next = newBlock;
				}

				*prev = curr->next;
				auto* allocatedBlock = reinterpret_cast<BlockHeader*>(alignedAddr - headerSize);
				allocatedBlock->size = size + headerSize;
				return reinterpret_cast<void*>(alignedAddr);
			}

			curr = curr->next;
		}

		return nullptr;
	}

	void Free(void* addr) noexcept
	{
		std::lock_guard lock(m_mutex);
		if (!addr)
		{
			return;
		}

		auto* block = reinterpret_cast<BlockHeader*>(reinterpret_cast<uintptr_t>(addr) - sizeof(BlockHeader));

		block->next = m_freeList;
		m_freeList = block;
	}

private:
	struct BlockHeader
	{
		size_t size;
		BlockHeader* next;
	};

	BlockHeader* m_freeList = nullptr;
	std::mutex m_mutex;
};
