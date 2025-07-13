#pragma once
#include <base_api.hpp>

namespace Mortis::PE 
{
	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	using Process32First = std::conditional_t<std::is_same_v<T, char>, 
		BT::StaticFunctorWrapper<::Process32First>, 
		BT::StaticFunctorWrapper<::Process32FirstW>>;

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	using Process32Next = std::conditional_t<std::is_same_v<T, char>, 
		BT::StaticFunctorWrapper<::Process32Next>, 
		BT::StaticFunctorWrapper<::Process32NextW>>;

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	using Module32First = std::conditional_t<std::is_same_v<T, char>,
		BT::StaticFunctorWrapper<::Module32First>,
		BT::StaticFunctorWrapper<::Module32FirstW>>;

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	using Module32Next = std::conditional_t<std::is_same_v<T, char>,
		BT::StaticFunctorWrapper<::Module32Next>,
		BT::StaticFunctorWrapper<::Module32NextW>>;

	template<typename AddressType = LPVOID>
	constexpr auto MakeAddress(auto address, auto offset) noexcept 
		-> LPVOID {
		return reinterpret_cast<AddressType>(reinterpret_cast<SIZE_T>(address) + offset);
	};

}
