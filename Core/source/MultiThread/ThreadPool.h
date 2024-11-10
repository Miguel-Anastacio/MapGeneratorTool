#pragma once
#include "SafeQueue.h"
#include <vector>
#include <thread>
#include <functional>
#include <future>
namespace Core
{ 
class ThreadPool
{
public:
	explicit ThreadPool(int numThreads) : stop(false)
	{
		for (std::size_t i = 0; i < numThreads; ++i)
		{
			workers.emplace_back([this] { this->workerThread();});
		}
	}

	~ThreadPool()
	{
		 //Task = {Execute/Stop, function, args}
		/*Task const stop_task{ TaskType::Stop, {}, {} };
		for (std::size_t i = 0; i < _threads.size(); ++i)
		{
			push(stop_task);
		}*/
		stop.store(true);
	}

	/*bool push(Task const& task)
	{
		m_safeQueue.push(task);
		return true;
	}*/

	template <class F, class... Args>
	auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>
	{
		using returnType = decltype(f(args...));

		auto task = std::make_shared<std::packaged_task<returnType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<returnType> res = task->get_future();
		m_safeQueue.push([task]() { (*task)(); });
		return res;
	}

private:
	//int m_threads;
	void workerThread();

	SafeQueue<std::function<void()>> m_safeQueue;
	std::vector<std::jthread> workers;
	std::atomic<bool> stop;

};

} // namespace Core