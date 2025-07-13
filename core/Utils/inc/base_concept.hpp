#pragma once
#include<base_template.hpp>
#include<string>
#include<string_view>
namespace Mortis::BC
{
	template<typename T1, typename... T2>
	concept HasType = (std::is_same_v<T1, T2> || ...);

	template <typename T>
	concept NotConst = not std::is_const_v<T>;

	template <typename T>
	concept NotRef = not std::is_reference_v<T>;

	template <typename T>
	concept NotPtr = not std::is_pointer_v<T>;

	template <typename StaticFunctorType>
	concept IsStaticFunctor = requires(StaticFunctorType func) {
		std::same_as<typename StaticFunctorType::value_type, decltype(func())>;
	};

	template<typename CallFunc, typename... Arg>
	concept CanCall = requires {
		requires (std::invocable<CallFunc, Arg> || ...);
	};

	template<typename T>
	concept IsString = requires {
		requires std::convertible_to<T, std::string_view> || std::convertible_to<T, std::wstring_view>;
	};

	template<typename T1, typename T2>
	concept ArrayElementTypeIsSame = requires(T1 t1, T2 t2) {
		requires std::same_as<std::remove_const_t<std::remove_reference_t<decltype(t1[0])>>, std::remove_const_t<std::remove_reference_t<decltype(t2[0])>>>;
	};

};