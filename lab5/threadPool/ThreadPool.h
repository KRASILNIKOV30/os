#include <atomic>
#include <condition_variable>
#include <exception>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(unsigned numThreads)
    {
        for (unsigned i = 0; i < numThreads; ++i)
        {
            workers.emplace_back([this](std::stop_token stopToken) {
                WorkerThread(stopToken);
            });
        }
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool()
    {
        Stop();
    }

    void Dispatch(Task task)
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (stopFlag)
            {
                return;
            }
            taskQueue.push(std::move(task));
        }
        condition.notify_one(); // уведомляем рабочие потоки о новой задаче
    }

    // Блокирует работу текущего потока до завершения всех задач
    void Wait()
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        isWaiting = true;
        condition.wait(lock, [this]() { return taskQueue.empty() && activeTasks == 0; });
    }

    // Останавливает пул и игнорирует все незавершенные задачи
    void Stop()
    {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stopFlag = true;
        }
        condition.notify_all(); // уведомляем все потоки о завершении работы
    }

private:
    // Рабочий поток
    void WorkerThread(std::stop_token stopToken)
    {
        while (!stopToken.stop_requested())
        {
            Task task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this]() { return stopFlag || !taskQueue.empty(); });

                if (stopFlag && taskQueue.empty())
                {
                    return;
                }

                task = std::move(taskQueue.front());
                taskQueue.pop();
                ++activeTasks;
            }

            try
            {
                task();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception caught in background task: " << e.what() << std::endl;
            }

            {
                std::lock_guard<std::mutex> lock(queueMutex);
                --activeTasks;
                if (isWaiting && taskQueue.empty() && activeTasks == 0)
                {
                    condition.notify_all(); // уведомляем основной поток, если задачи завершены
                }
            }
        }
    }

    std::queue<Task> taskQueue;           
    std::mutex queueMutex;                
    std::condition_variable condition;    
    std::atomic<bool> stopFlag;           
    std::atomic<int> activeTasks = 0;     
    bool isWaiting;                       
    std::vector<std::jthread> workers;    
};
