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
		constexpr void emplace_back(Args&&... args) {
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

		constexpr bool is_single() const noexcept {
			return std::holds_alternative<T>(*this);
		}

		constexpr bool is_multiple() const noexcept {
			return std::holds_alternative<std::vector<T>>(*this);
		}

		constexpr T& get_single() {
			return std::get<T>(*this);
		}

		constexpr const T& get_single() const {
			return std::get<T>(*this);
		}

		constexpr std::vector<T>& get_multiple() {
			return std::get<std::vector<T>>(*this);
		}

		constexpr const std::vector<T>& get_multiple() const {
			return std::get<std::vector<T>>(*this);
		}

	};

	struct WordVariant : public std::variant<BYTE, WORD, DWORD32, DWORD64>
	{
		constexpr std::size_t get_size() const { return 1ULL << this->index(); }
		template<typename T>
		constexpr T& get() {
			 std::get<T>(*this);
		}
		template<typename T>
		constexpr const T& get() const {
			return std::get<T>(*this);
		}

	};

	template<typename T = void>
	using Expected = std::expected<T, std::string_view>;

	using Expected_bool = Expected<bool>;
	using UnExpected = std::unexpected<std::string_view>;


	enum CodePage : unsigned int
	{
		UTF_8 = CP_UTF8,
		UTF_16 = 1200,

		UTF_16BE = 1201,
		UTF_32 = 12000,

		GB2312 = 936,
		SHIFT_JIS = 932,
	};

	enum CallingConvention 
	{
		X86_CALL = 0x01,
		X86_CDECL = 0x02,
		
		X64_CALL = 0x04,
		X64_CDECL = 0x08, 
	};

}
