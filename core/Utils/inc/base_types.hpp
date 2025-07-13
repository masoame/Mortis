#pragma once
#include<type_traits>
#include<string>
#include<string_view>
namespace Mortis 
{
	template <typename T>
	struct SingleOrMultiple : public std::variant<T, std::vector<T>>
	{
		template<typename ...Args>
		void emplace_back(Args&&... args) {
			if (std::holds_alternative<T>(*this)) {
				std::vector<T> multiple_values;
				multiple_values.emplace_back(std::move(std::get<T>(*this)));
				multiple_values.emplace_back(std::forward<Args>(args)...);
				std::variant<T, std::vector<T>>::emplace<1>(std::move(multiple_values));
			}
			else {
				std::get<std::vector<T>>(*this).emplace_back(std::forward<Args>(args)...);
			}
		}
	};

	enum class CodePage : unsigned int
	{
		UTF_8 = CP_UTF8,
		UTF_16 = 1200,
		UTF_16BE = 1201,
		UTF_32 = 12000,

		GB2312 = 936,
		SHIFT_JIS = 932,
	};
}
