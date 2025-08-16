#pragma once
#include<utils.hpp>

namespace Mortis
{
	template<class T>
	class bounded_queue
	{
		using Type = std::decay_t<T>;
	public:
		explicit bounded_queue(std::size_t max_size = ULLONG_MAX) : _max_size(max_size), _is_closed(false) {}

		~bounded_queue() noexcept {
			_is_closed = true;
			_cv_could_push.notify_all();
			_cv_could_pop.notify_all();
		}

		template<typename... Args>
		void emplace(Args&&... args) {
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, 
				[this] { 
					return (_queue.size() < this->_max_size) || _is_closed; 
				});

			if (_is_closed) {
				return;
			}
			_queue.emplace_back(std::forward<Args>(args)...);
			_cv_could_pop.notify_one();
			return;
		}



		void push(const Type& value) noexcept {
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, 
				[this] { 
					return (_queue.size() < this->_max_size) || _is_closed; 
				});

			if (_is_closed) {
				return;
			}
			_queue.push_back(value);
			_cv_could_pop.notify_one();
		}

		std::optional<Type> pop() noexcept {
			std::unique_lock lock(_mtx);
			_cv_could_pop.wait(lock, 
				[this] { 
					return (this->_queue.empty() == false) || _is_closed; 
				});

			if (_is_closed) {
				return std::nullopt;
			}
			Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		template <class _Rep, class _Period>
		std::optional<Type> pop_for(const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept{
			std::unique_lock lock(_mtx);
			bool cv_status = _cv_could_pop.wait_for(lock, _Rel_time, 
				[this] { 
					return (this->_queue.empty() == false) || _is_closed; 
				});

			if (_is_closed || !cv_status) {
				return std::nullopt;
			}
			Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		inline size_t size() const noexcept{
			return _queue.size();
		}

		inline bool empty() const noexcept{
			return _queue.empty();
		}

		inline bool full() const noexcept{
			return _queue.size() >= _max_size;
		}

		void clear() noexcept{
			std::unique_lock lock(_mtx);
			_queue.clear();
		}
		std::mutex _mtx;
	private:
		bool _is_closed = true;
		std::condition_variable _cv_could_push, _cv_could_pop;
		std::deque<Type> _queue;
		const size_t _max_size;
	};
}