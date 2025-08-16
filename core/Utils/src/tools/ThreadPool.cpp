#include <ThreadPool.hpp>

using namespace Mortis;
// the constructor just launches some amount of workers
ThreadPool::ThreadPool(size_t threads)
{
	for (size_t i = 0; i < threads; ++i) {
		workers.emplace_back([this]() {
			while (true){
				std::function<void()> task;
				{
					std::unique_lock<std::mutex> lock(this->queue_mutex);
					this->condition.wait(lock,
						[this] { return (this->stop.stop_requested() == true) || this->tasks.empty() == false; });
					if (this->stop.stop_requested() && this->tasks.empty())
						return;
					task = std::move(this->tasks.front());
					this->tasks.pop();
				}
				task();
			}
		});
	}
}

// the destructor joins all threads
ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop.request_stop();
	}
	condition.notify_all();
}