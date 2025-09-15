#pragma once
#include<utils.hpp>

namespace Mortis
{
	/*
	* BoundedQueue用于实现一个有界队列，支持线程安全的入队和出队操作。内部资源都是通过智能指针管理，支持自定义的删除函数。
	*/
	template<class _T>
	class bounded_queue
	{
		using _Type = std::remove_reference_t<_T>;
	public:
		explicit bounded_queue(size_t max_size = -1) : _max_size(max_size), _is_closed(false) {}

		~bounded_queue() {
			_is_closed = true;
			_cv_could_push.notify_all();
			_cv_could_pop.notify_all();
		}

		template<typename... Args>
		void emplace(Args&&... args) noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, [this] { return (_queue.size() < this->_max_size) || _is_closed; });
			if (_is_closed) {
				return;
			}
			_queue.emplace_back(std::forward<Args>(args)...);
			_cv_could_pop.notify_one();
			return;
		}

		void push(_Type&& value) noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, [this] { return (_queue.size() < this->_max_size) || _is_closed; });
			if (_is_closed) {
				return;
			}
			_queue.push_back(std::move(value));
			_cv_could_pop.notify_one();
		}

		void push(const _Type& value) noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, [this] { return (_queue.size() < this->_max_size) || _is_closed; });
			if (_is_closed) {
				return;
			}
			_queue.push_back(value);
			_cv_could_pop.notify_one();
		}

		std::optional<_Type> pop() noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_pop.wait(lock, [this] { return (this->_queue.empty() == false) || _is_closed; });
			if (_is_closed) {
				return std::nullopt;
			}
			_Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		template <class _Rep, class _Period>
		std::optional<_Type> pop_for(const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept
		{
			std::unique_lock lock(_mtx);
			bool cv_status = _cv_could_pop.wait_for(lock, _Rel_time, [this] { return (this->_queue.empty() == false) || _is_closed; });
			if (_is_closed || !cv_status) {
				return std::nullopt;
			}
			_Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		constexpr void setMaxSize(size_t new_size) noexcept 
		{
			std::unique_lock lock(_mtx);
			if (_queue.size() > new_size) {
				_queue.resize(new_size);
			}
			_max_size = new_size;
			_cv_could_push.notify_all();
		}

		constexpr size_t size() const noexcept
		{
			return _queue.size();
		}

		constexpr bool empty() const noexcept
		{
			return _queue.empty();
		}

		constexpr bool full() const noexcept
		{
			return _queue.size() >= _max_size;
		}

		constexpr void lock() noexcept
		{
			_mtx.lock();
		}

		constexpr void unlock() noexcept
		{
			_mtx.unlock();
		}

		constexpr void clear() noexcept
		{
			_queue.clear();
		}
		bool _is_closed = true;
		std::condition_variable _cv_could_push, _cv_could_pop;
		std::mutex _mtx;
	private:
		std::deque<_Type> _queue;
		mutable std::size_t _max_size;
	};
}