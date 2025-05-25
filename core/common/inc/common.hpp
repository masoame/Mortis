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



