#pragma once
#include<Utils.hpp>
namespace Mortis
{
	/*
	* singletont_thread_pool用于实现一个单例线程池，支持线程安全的任务队列和线程管理。
	* 参考项目来源: https://github.com/progschj/ThreadPool
	* 适配C++20，并增加stop_source参数，用于控制线程池的关闭。
	* 使用std::jthread替代原来的std::thread，支持stop_token参数，用于控制线程的关闭。
	*/
	class ThreadPool {

	public:
		explicit ThreadPool(size_t threads);
		~ThreadPool();

		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
			-> std::future<typename std::invoke_result<F, Args...>::type>;
	private:

		// need to keep track of threads so we can join them
		std::vector< std::jthread > workers{};
		// the task queue
		std::queue< std::function<void()>> tasks{};

		// synchronization
		std::mutex queue_mutex{};
		std::condition_variable condition{};
		//bool stop;
		std::stop_source stop{};
	};

	// add new work item to the pool
	template<class F, class... Args>
	auto ThreadPool::enqueue(F&& f, Args&&... args)
		-> std::future<typename std::invoke_result<F, Args...>::type>
	{
		using return_type = typename std::invoke_result<F, Args...>::type;

		auto task = std::make_shared<std::packaged_task<return_type()> >(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			if (stop.stop_requested()) {
				throw std::runtime_error("enqueue on stopped ThreadPool");
			}
			tasks.emplace([task]() { (*task)(); });
		}
		condition.notify_one();
		return res;
	}
}


