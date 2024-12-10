#pragma once
#include <chrono>

class Timer
{
	using Clock = std::chrono::high_resolution_clock;

public:
	Timer() = default;
	Timer(const Clock&) = delete;
	Clock& operator=(const Clock&) = delete;

	double Restart()
	{
		const auto cur = Clock::now();
		const auto dur = cur - m_start;
		m_start = cur;
		return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(dur).count()) / 1e6;
	}

private:
	Clock::time_point m_start = Clock::now();
};