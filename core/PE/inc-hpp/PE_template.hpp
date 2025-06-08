#pragma once
#include <PE_concept.hpp>
#include <PE_api.hpp>
#include <PE_types.hpp>
namespace Mortis::BT
{
	template<typename T1>
	struct SearchModuleWrapper {
		inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
		using TYPE = PE::TYPE<is_wide>;
		using API = PE::API<is_wide>;
	};

	template<typename T1>
	struct SearchProcessWrapper {
		inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
		using TYPE = PE::TYPE<is_wide>;
		using API = PE::API<is_wide>;
	};

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	struct ProcessInfoWrapper {
		inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
		using TYPE = PE::TYPE<is_wide>;
		using API = PE::API<is_wide>;
	};

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	struct ModuleInfoWrapper {
		inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
		using TYPE = PE::TYPE<is_wide>;
		using API = PE::API<is_wide>;
	};
}
