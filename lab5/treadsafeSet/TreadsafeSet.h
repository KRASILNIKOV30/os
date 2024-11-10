#pragma once
#include "ISet.h"
#include <unordered_set>
#include <functional>
#include <mutex>
#include <shared_mutex>

template<typename T, typename H = std::hash<T>, typename Comp = std::equal_to<T>>
class ThreadsafeSet : public ISet<T>
{
public:
	void Insert(const T& value) override
	{
		// чем отличается от lock_guard
		std::unique_lock lock(m_mutex);
		m_set.insert(value);
	}

	// можно вернуть конт ссылку
	T Get(size_t i) const override
	{
		//сделать получение всех элементов за O(1)
		std::shared_lock lock(m_mutex);
		return *std::next(m_set.cbegin(), i);
	}

	//Посмотреть закон Амдала и прикинуть, сколько времени занимает ожидание

private:
	std::unordered_set<T, H, Comp> m_set;
	mutable std::shared_mutex m_mutex{};
};

template<typename T, typename H = std::hash<T>, typename Comp = std::equal_to<T>>
class Set : public ISet<T>
{
public:
	void Insert(const T& value) override
	{
		m_set.insert(value);
	}

	T Get(size_t i) const override
	{
		return *std::next(m_set.cbegin(), i);
	}

private:
	std::unordered_set<T, H, Comp> m_set;
};