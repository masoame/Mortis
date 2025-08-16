#pragma once
#include<utils.hpp>
#include<string_view>
namespace Mortis
{
	inline std::wstring MultiStringToWideString(UINT CodePage, std::string_view str)
	{
		std::wstring result(MultiByteToWideChar(CodePage, 0, str.data(), -1, nullptr, 0), L'\0');
		MultiByteToWideChar(CodePage, 0, str.data(), -1, result.data(), static_cast<int>(result.size()));
		return result;
	}

	inline std::string WideStringToMultiString(UINT CodePage, std::wstring_view str)
	{
		std::string result(WideCharToMultiByte(CodePage, 0, str.data(), -1, nullptr, 0, NULL, NULL), '\0');
		WideCharToMultiByte(CodePage, 0, str.data(), -1, result.data(), static_cast<int>(result.size()), NULL, NULL);
		return result;
	}

	constexpr static auto UTF8ToUTF16 = std::bind(MultiStringToWideString, CP_UTF8, std::placeholders::_1);
	constexpr static auto ANSIToUTF16 = std::bind(MultiStringToWideString, CP_ACP, std::placeholders::_1);
	constexpr static auto GBKToUTF16 = std::bind(MultiStringToWideString, 936, std::placeholders::_1);

	constexpr static auto UTF16ToUTF8 = std::bind(WideStringToMultiString, CP_UTF8, std::placeholders::_1);
	constexpr static auto UTF16ToANSI = std::bind(WideStringToMultiString, CP_ACP, std::placeholders::_1);
	constexpr static auto UTF16ToGBK = std::bind(WideStringToMultiString, 936, std::placeholders::_1);


	template<typename CharType>
	constexpr auto ToUpperCaseStdString(std::basic_string_view<CharType> str_view)
		-> std::basic_string<CharType>
	{
		std::basic_string<CharType>  result;
		result.resize(str_view.size());
		std::ranges::transform(str_view, result.begin(),
			[](CharType c) {
				return static_cast<CharType>(std::toupper(c));
			});
		return result;
	}

	template<typename CharType>
	constexpr auto ToLowerCaseStdString(std::basic_string_view<CharType> str_view)
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
	constexpr bool CaseInsensitiveCompare(std::basic_string_view<CharType> str_view1, std::basic_string_view<CharType> str_view2)
	{
		return ToUpperCaseStdString<CharType>(str_view1) == ToUpperCaseStdString<CharType>(str_view2);
	}

	template<typename CharType>
	struct CaseInsensitiveStdString : std::basic_string<CharType>
	{
		template<typename... Args>
		constexpr CaseInsensitiveStdString(Args&&... args) :
			std::basic_string<CharType>(std::forward<Args>(args)...),
			_lowerStdString(ToLowerCaseStdString<CharType>(dynamic_cast<std::basic_string<CharType>&>(*this)))
		{
		}

		constexpr operator std::basic_string_view<CharType>() const {
			return dynamic_cast<const std::basic_string<CharType>&>(*this);
		}

		constexpr std::basic_string_view<CharType> view() const {
			return dynamic_cast<const std::basic_string<CharType>&>(*this);
		}

		constexpr auto operator <=> (const CaseInsensitiveStdString& other) const noexcept {
			return _lowerStdString <=> other._lowerStdString;
		}

		constexpr void releaseToStdString(std::basic_string<CharType>& target) {
			target = std::move(dynamic_cast<std::basic_string<CharType>&>(*this));
			_lowerStdString.clear();
		}

		constexpr void release(std::basic_string<CharType>& target) {
			dynamic_cast<std::basic_string<CharType>&>(*this).clear();
			_lowerStdString.clear();
		}

		mutable std::basic_string<CharType> _lowerStdString;
	};
}



