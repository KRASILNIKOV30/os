#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Mutex
{
public:
	void lock() noexcept
	{
		while (m_flag.test_and_set(std::memory_order_acquire))
		{
			// m_flag.wait(true, std::memory_order_relaxed);
		}
	}

	bool try_lock() noexcept
	{
		return !m_flag.test_and_set(std::memory_order_acquire);
	}

	void unlock() noexcept
	{
		m_flag.clear(std::memory_order_release);
		// m_flag.notify_one();
	}

private:
	std::atomic_flag m_flag{};
};

static Mutex m;
static int out = 0;

int main()
{
	auto t1 = std::jthread([&]() {
        for (int i = 0; i < 1000000; i++)
        {
            std::lock_guard lock{ m };
            ++out;
        }
    });

    auto t2 = std::jthread([&]() {
        for (int i = 0; i < 1000000; i++)
        {
            std::lock_guard lock{ m };
            --out;
        }
    });
	
    t1.join();
    t2.join();

    std::cout << out << std::endl;

    return 0;
}