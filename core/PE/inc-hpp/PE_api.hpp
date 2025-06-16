#pragma once
#include <base_api.hpp>

namespace Mortis::PE 
{
	template<bool is_wide>
	struct API : public BaseAPI<is_wide> {
		using Process32FirstWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Process32FirstW>, BT::StaticFunctorWrapper<Process32First>>;
		using Process32NextWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Process32NextW>, BT::StaticFunctorWrapper<Process32Next>>;
		using Module32FirstWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Module32FirstW>, BT::StaticFunctorWrapper<Module32First>>;
		using Module32NextWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Module32NextW>, BT::StaticFunctorWrapper<Module32Next>>;
	};

	template<typename AddressType = LPVOID>
	inline auto MakeAddress(auto address, auto offset) noexcept 
		-> LPVOID {
		return reinterpret_cast<AddressType>(reinterpret_cast<SIZE_T>(address) + offset);
	};

}
