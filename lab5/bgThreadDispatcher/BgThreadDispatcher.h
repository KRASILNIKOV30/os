#pragma once
#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class BgThreadDispatcher
{
public:
	using Task = std::function<void()>;

	BgThreadDispatcher()
	{
		m_workerThread = std::jthread([this](const std::stop_token& stopToken) {
			WorkerThread(stopToken);
		});
	}

	BgThreadDispatcher(const BgThreadDispatcher&) = delete;
	BgThreadDispatcher& operator=(const BgThreadDispatcher&) = delete;

	~BgThreadDispatcher()
	{
		Stop();
	}

	void Dispatch(Task task)
	{
		if (m_stopFlag)
		{
			return;
		}
		{
			std::lock_guard lock(m_queueMutex);
			m_tasks.push(std::move(task));
		}
		// убрать лок (исправлено)
		m_condVar.notify_one();
	}

	void Wait()
	{
		std::unique_lock lock(m_queueMutex);
		// Написать тест: два потока делают wait,
		m_condVar.wait(lock, [this]() { return m_tasks.empty() && !m_isWorking; });
	}

	void Stop()
	{
		// ЛОк не нужен (Исправлено)
		m_stopFlag = true;
		m_condVar.notify_one();
	}

private:
	void WorkerThread(std::stop_token const& stopToken)
	{
		while (!stopToken.stop_requested())
		{
			Task task;
			{
				std::unique_lock lock(m_queueMutex);
				m_condVar.wait(lock, [this]() { return m_stopFlag || !m_tasks.empty(); });

				if (m_stopFlag && m_tasks.empty())
				{
					break;
				}

				task = std::move(m_tasks.front());
				m_tasks.pop();
				m_isWorking = true;
			}

			try
			{
				task();
			}
			catch (const std::exception& e)
			{
				std::cerr << "Exception caught in background task: " << e.what() << std::endl;
			}

			m_isWorking = false;
			if (m_tasks.empty())
			{
				m_condVar.notify_all();
			}
		}
	}

	std::queue<Task> m_tasks;
	std::mutex m_queueMutex;
	// переименовать (Исправлено)
	std::condition_variable m_condVar;
	std::atomic<bool> m_stopFlag = false;
	std::atomic<bool> m_isWorking = false;
	std::jthread m_workerThread;
};
