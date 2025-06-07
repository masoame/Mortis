#pragma once
#include<type_traits>
#include<base_template.hpp>
namespace Mortis
{
	template<auto is_wide>
	struct BaseAPI
	{
		using StrCmpWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<wcscmp>, BT::StaticFunctorWrapper<strcmp>>;
		using StrCmpIgnoreCaseWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<wcsicmp>, BT::StaticFunctorWrapper<stricmp>>;
	};
}
