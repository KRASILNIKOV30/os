#pragma once
#include <mutex>
#include <shared_mutex>
#include <vector>

template<typename T>
class ThreadsafeVector
{
public:
	void PushBack(const T& value)
	{
		std::unique_lock lock(m_mutex);
		m_vector.push_back(value);
	}

	typename std::vector<T>::const_iterator begin() const
	{
		return m_vector.cbegin();
	}

	typename std::vector<T>::const_iterator end() const
	{
		return m_vector.cend();
	}

private:
	std::shared_mutex m_mutex;
	std::vector<T> m_vector;
};
