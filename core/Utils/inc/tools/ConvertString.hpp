#pragma once
#include<utils.hpp>
#include<string_view>
namespace Mortis
{
	template<typename CharType> 
	auto ToUpperCaseStdString(std::basic_string_view<CharType> str_view)
		-> std::basic_string<CharType> 
	{
		std::basic_string<CharType>  result;
		result.resize(str_view.size());
		std::ranges::transform(str_view, result.begin(),
			[](CharType c){ 
				return static_cast<CharType>(std::toupper(c)); 
			});
		return result;
	}

	template<typename CharType>
	auto ToLowerCaseStdString(std::basic_string_view<CharType> str_view)
		-> std::basic_string<CharType> 
	{
		std::basic_string<CharType> result;
		result.resize(str_view.size());
		std::ranges::transform(str_view, result.begin(),
			[](CharType c)->CharType { 
				return static_cast<CharType>(std::tolower(c)); 
			});
		return result;
	}

	template<typename CharType>
	bool CaseInsensitiveCompare(std::basic_string_view<CharType> str_view1, std::basic_string_view<CharType> str_view2)
	{
		return ToUpperCaseStdString<CharType>(str_view1) == ToUpperCaseStdString<CharType>(str_view2);
	}
}



