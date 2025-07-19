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
			[](CharType c) {
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

	template<typename CharType>
	struct CaseInsensitiveStdString : std::basic_string<CharType>
	{
		template<typename... Args>
		CaseInsensitiveStdString(Args&&... args) :
			std::basic_string<CharType>(std::forward<Args>(args)...),
			_lowerStdString(ToLowerCaseStdString<CharType>(dynamic_cast<std::basic_string<CharType>&>(*this)))
		{
		}

		operator std::basic_string_view<CharType>() const {
			return dynamic_cast<const std::basic_string<CharType>&>(*this);
		}

		std::basic_string_view<CharType> view() const {
			return dynamic_cast<const std::basic_string<CharType>&>(*this);
		}

		auto operator <=> (const CaseInsensitiveStdString& other) const noexcept {
			return _lowerStdString <=> other._lowerStdString;
		}

		void releaseToStdString(std::basic_string<CharType>& target) {
			target = std::move(dynamic_cast<std::basic_string<CharType>&>(*this));
			_lowerStdString.clear();
		}

		void release(std::basic_string<CharType>& target) {
			dynamic_cast<std::basic_string<CharType>&>(*this).clear();
			_lowerStdString.clear();
		}

		mutable std::basic_string<CharType> _lowerStdString;
	};
}



