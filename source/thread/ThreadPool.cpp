#include "ThreadPool.h"
namespace thread
{
void ThreadPool::workerThread()
{
    while (!stop.load()) 
    {
        std::function<void()> task;
        if (auto optTask = m_safeQueue.tryPop(); optTask) 
        {
            task = *optTask;
            task();
        } 
        else 
        {
            std::this_thread::yield(); // Yield CPU if no task available
        }
    }
}



	//ThreadPool::ThreadPool(int numThreads)
//{
//
//}
//ThreadPool::~ThreadPool()
//{
//
//}
} // namespace thread