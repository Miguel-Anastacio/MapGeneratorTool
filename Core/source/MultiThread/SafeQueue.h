#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>
#include <optional>
namespace Core
{
template<typename T>
class SafeQueue
{
public:
	void push(T const& val)
	{
		std::lock_guard<std::mutex> lock(mtx);
		m_queue.push(std::move(val));
		cv.notify_one(); // Notify one waiting thread that data is available
	}

	T pop()
	{
		std::unique_lock<std::mutex> uLock(mtx);
		cv.wait(uLock, [&] { return !m_queue.empty(); });
		T front = m_queue.front();
		m_queue.pop();
		return front;
	}

	std::optional<T> tryPop() 
	{
		std::lock_guard<std::mutex> lock(mtx);
		if (m_queue.empty())
		{
			return std::nullopt; // Return empty optional if no item is available
		}
		T value = std::move(m_queue.front());
		m_queue.pop();
		return value;
	}

	bool empty() const 
	{
		std::lock_guard<std::mutex> lock(mtx);
		return m_queue.empty();
	}

private:
	std::queue<T> m_queue; // Underlying queue to store elements
	std::condition_variable cv; // Condition variable for synchronization
	std::mutex mtx;		// Mutex for exclusive access to the queue
};
} // namespace thread
