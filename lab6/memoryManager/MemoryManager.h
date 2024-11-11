#pragma once
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>

class MemoryManager
{
public:
	MemoryManager(void* start, size_t size)
		: m_start(static_cast<uint8_t*>(start))
		, m_size(size)
		, m_freeList(nullptr)
	{
		if (reinterpret_cast<std::uintptr_t>(m_start) % alignof(std::max_align_t) != 0)
		{
			throw std::invalid_argument("Start address must be aligned to std::max_align_t");
		}

		m_freeList = reinterpret_cast<BlockHeader*>(m_start);
		m_freeList->size = size;
		m_freeList->next = nullptr;
	}

	MemoryManager(const MemoryManager&) = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

	void* Allocate(size_t size, size_t align = alignof(std::max_align_t)) noexcept
	{
		if (size == 0 || align == 0 || (align & (align - 1)) != 0)
		{
			return nullptr;
		}

		BlockHeader** prev = &m_freeList;
		BlockHeader* curr = m_freeList;

		while (curr != nullptr)
		{
			uintptr_t addr = reinterpret_cast<uintptr_t>(curr) + sizeof(BlockHeader);
			size_t alignedAddr = (addr + align - 1) & ~(align - 1);

			size_t totalSize = alignedAddr - reinterpret_cast<uintptr_t>(curr);

			if (curr->size >= totalSize + size)
			{
				// Блок подходит, выделяем память

				// Разделяем блок, если оставшееся пространство достаточно для нового блока
				if (curr->size >= totalSize + size + sizeof(BlockHeader))
				{
					BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(alignedAddr + size);
					newBlock->size = curr->size - totalSize - size;
					newBlock->next = curr->next;

					// Переносим указатель на новый свободный блок
					curr->size = totalSize;
					curr->next = newBlock;
				}

				// Удаляем блок из списка свободных
				*prev = curr->next;
				return reinterpret_cast<void*>(alignedAddr);
			}

			prev = &curr->next;
			curr = curr->next;
		}

		return nullptr;
	}

	void Free(void* addr) noexcept
	{
		if (!addr)
		{
			return;
		}

		BlockHeader* block = reinterpret_cast<BlockHeader*>(reinterpret_cast<uintptr_t>(addr) - sizeof(BlockHeader));

		block->next = m_freeList;
		m_freeList = block;
	}

private:
	struct BlockHeader
	{
		size_t size; // Размер блока памяти
		BlockHeader* next; // Указатель на следующий свободный блок
	};

	uint8_t* m_start; // Начало блока памяти
	size_t m_size; // Размер блока памяти
	BlockHeader* m_freeList; // Список свободных блоков
};
