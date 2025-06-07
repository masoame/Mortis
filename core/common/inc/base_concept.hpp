#pragma once
#include<type_traits>
#include<string>
#include<string_view>
namespace Mortis::BC
{
	template<typename T1, typename... T2>
	concept HasType = requires {
		requires (std::same_as<T1, T2> || ...);
	};

	template<typename T1, typename... T2>
	concept CanCall = requires {
		requires (std::invocable<T1, T2> || ...);
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