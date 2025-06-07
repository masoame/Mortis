#pragma once
#include<type_traits>
#include<string>
#include<string_view>
namespace Mortis 
{
	template<auto is_wide>
	struct BaseTYPE
	{
		using CharWrapper = std::conditional_t<is_wide, wchar_t, char>;
		using StringViewWrapper = std::conditional_t<is_wide, std::wstring_view, std::string_view>;
		using StringWrapper = std::conditional_t<is_wide, std::wstring, std::string>;
	};
}
