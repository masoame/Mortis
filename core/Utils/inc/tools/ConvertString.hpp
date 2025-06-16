#pragma once
#include<utils.hpp>
#include<string_view>
namespace Mortis
{
	template<typename CharType, 
		typename StrType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>
	> requires BC::IsSameType<CharType, char, wchar_t, char8_t, char16_t, char32_t>
	auto ToUpperCaseStdString(std::basic_string_view<CharType> str_view)
		-> StrType {
		StrType result;
		result.resize(str_view.size());
		std::ranges::transform(str_view, result.begin(),
			[](CharType c){ 
				return static_cast<CharType>(std::toupper(c)); 
			});
		return result;
	}

	template<typename CharType,
		typename StrType = std::basic_string<CharType, std::char_traits<CharType>, std::allocator<CharType>>
	> requires BC::IsSameType<CharType, char, wchar_t, char8_t, char16_t, char32_t>
	auto ToLowerCaseStdString(std::basic_string_view<CharType> str_view)
		-> StrType {
		StrType result;
		result.resize(str_view.size());
		std::ranges::transform(str_view, result.begin(),
			[](CharType c)->CharType { 
				return static_cast<CharType>(std::tolower(c)); 
			});
		return result;
	}

	template<typename CharType>
	auto CaseInsensitiveCompare(std::basic_string_view<CharType> str_view1, std::basic_string_view<CharType> str_view2)
		-> bool {
		return ToUpperCaseStdString<CharType>(str_view1) == ToUpperCaseStdString<CharType>(str_view2);
	}

#ifdef _WIN32


#endif // _WIN32
}



