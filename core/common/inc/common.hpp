#pragma once

#ifdef _WIN32
#include<Windows.h>
#include<tlhelp32.h>
#else

#endif
#include<functional>
#include<sstream>
#include<optional>
#include<string>
#include<vector>
#include<memory>
#include<map>
#include<type_traits>
#include<mutex>
#include<thread>
#include<queue>

namespace Mortis
{
	template <auto _F>
	using Functor = std::integral_constant<std::remove_reference_t<decltype(_F)>, _F>;

	namespace BaseConcept 
	{
		template<typename T1, typename... T2>
		concept HasType = requires {
			requires (std::same_as<T1, T2> || ...);
		};

		template<typename T>
		concept IsString = requires {
			requires std::convertible_to<T, std::string> || std::convertible_to<T, std::wstring> || HasType<T, std::string_view, std::wstring_view>;
		};

		template<typename T1, typename T2>
		concept ArrayElementTypeIsSame = requires(T1 t1, T2 t2) {
			requires std::same_as<std::remove_const_t<std::remove_reference_t<decltype(t1[0])>>, std::remove_const_t<std::remove_reference_t<decltype(t2[0])>>>;
		};

	};

	template<auto is_wide>
	struct BaseAPI 
	{
		using StrCmpWrapper = std::conditional_t<is_wide, Functor<wcscmp>, Functor<strcmp>>;
		using StrCmpIgnoreCaseWrapper = std::conditional_t<is_wide, Functor<wcsicmp>, Functor<stricmp>>;
	};
	
	template<auto is_wide>
	struct BaseTYPE
	{

	};


	template<class _T = void, class _FreeFunc = Functor<CloseHandle>>
	struct AutoHandle
	{
		using _Type = std::remove_reference_t<_T>;
		static_assert(std::is_invocable_v<typename _FreeFunc::value_type, _Type**> || std::is_invocable_v<typename _FreeFunc::value_type, _Type*>);
		constexpr static bool isSecPtr = not std::is_invocable_v<typename _FreeFunc::value_type, _Type*>;

		AutoHandle() noexcept {}
		AutoHandle(AutoHandle& _handle) = delete;
		AutoHandle(AutoHandle&& _handle) noexcept : _ptr(_handle.release()) {}
		AutoHandle(_Type* ptr) noexcept : _ptr(ptr) {}

		AutoHandle& operator=(_Type* ptr) noexcept { _ptr.reset(ptr); return *this; }
		AutoHandle& operator=(AutoHandle&& _handle) noexcept { _ptr.reset(_handle.release()); return *this; }

		operator const _Type* () const noexcept { return _ptr.get(); }
		operator _Type*& () noexcept { return *reinterpret_cast<_Type**>(this); }
		operator bool() const noexcept { return _ptr.get() != nullptr; }

		_Type** operator&() { static_assert(sizeof(*this) == sizeof(void*)); return reinterpret_cast<_Type**>(this); }

		_Type* operator->() const noexcept { return _ptr.get(); }

		void reset(_Type* ptr = nullptr) noexcept { _ptr.reset(ptr); }

		auto release() noexcept { return _ptr.release(); }
		auto get() const noexcept { return _ptr.get(); }
	private:

		struct DeletePrimaryPtr { void operator()(void* ptr) { _FreeFunc()(static_cast<_Type*>(ptr)); } };
		struct DeleteSecPtr { void operator()(void* ptr) { _FreeFunc()(reinterpret_cast<_Type**>(&ptr)); } };

		using DeletePtr = std::conditional<isSecPtr, DeleteSecPtr, DeletePrimaryPtr>::type;
		std::unique_ptr<_Type, DeletePtr> _ptr;
	};


	template<class _T>
	class bounded_queue
	{
		using Type = std::remove_reference_t<_T>;
	public:

		bounded_queue(bounded_queue&) = default;
		bounded_queue(bounded_queue&&) = default;

		bounded_queue(std::size_t max_size = ULLONG_MAX) : _max_size(max_size), _is_closed(false) {}

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
			if (_is_closed) return;
			_queue.emplace_back(std::forward<Args>(args)...);
			_cv_could_pop.notify_one();
			return;
		}

		void push(Type&& value) noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, [this] { return (_queue.size() < this->_max_size) || _is_closed; });
			if (_is_closed) return;
			_queue.push_back(std::move(value));
			_cv_could_pop.notify_one();
		}

		void push(const Type& value) noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_push.wait(lock, [this] { return (_queue.size() < this->_max_size) || _is_closed; });
			if (_is_closed) return;
			_queue.push_back(value);
			_cv_could_pop.notify_one();
		}

		std::optional<Type> pop() noexcept
		{
			std::unique_lock lock(_mtx);
			_cv_could_pop.wait(lock, [this] { return (this->_queue.empty() == false) || _is_closed; });
			if (_is_closed) return std::nullopt;
			Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		template <class _Rep, class _Period>
		std::optional<Type> pop_for(const std::chrono::duration<_Rep, _Period>& _Rel_time) noexcept
		{
			std::unique_lock lock(_mtx);
			bool cv_status = _cv_could_pop.wait_for(lock, _Rel_time, [this] { return (this->_queue.empty() == false) || _is_closed; });
			if (_is_closed || !cv_status) return std::nullopt;
			Type _ret{ std::move(_queue.front()) };
			_queue.pop_front();
			_cv_could_push.notify_one();
			return _ret;
		}

		inline size_t size() const noexcept
		{
			return _queue.size();
		}

		inline bool empty() const noexcept
		{
			return _queue.empty();
		}

		inline bool full() const noexcept
		{
			return _queue.size() >= _max_size;
		}

		void clear() noexcept
		{
			std::unique_lock lock(_mtx);
			_queue.clear();
		}
	private:
		bool _is_closed = true;
		std::condition_variable _cv_could_push, _cv_could_pop;
		std::deque<Type> _queue;
		const std::size_t _max_size;
		std::mutex _mtx;
	};
}

namespace os
{
	inline std::optional<std::wstring> GetModuleFileNameW(HMODULE module = NULL)
	{
		std::wstring buffer(MAX_PATH, L'\0');
		if (GetModuleFileNameW(module, buffer.data(), MAX_PATH)) return buffer;
		return {};
	}
}



