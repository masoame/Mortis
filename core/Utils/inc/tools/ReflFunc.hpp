#pragma once
#include<utils.hpp>

namespace Mortis::PE
{
	template<typename FuncPtrType>
	struct ReflFunc
	{
		constexpr static FuncPtrType _function_address;

		constexpr ReflFunc(FuncPtrType fun_ptr) {
			_function_address = fun_ptr;
		}

		template<typename Type>
		constexpr auto refl_args_positions() const noexcept {
			return BT::get_type_positions<Type>(_function_address);
		}

		template<typename... Type>
		constexpr auto refl_args_positions_tuple() const noexcept {
			return std::make_tuple(
				std::pair{ typeid(Type).name(), refl_args_positions<Type>() }...
			);
		}

		template<typename... Type>
		auto refl_args_positions_map() const noexcept {
			std::map<std::string_view, std::vector<std::size_t>> result;
			std::apply([&result](auto&& ...args) {
				(result.insert({ args.first , std::vector<std::size_t>(args.second.cbegin(),args.second.cend()) }), ...);
				}, refl_args_positions_tuple<Type...>());
			return result;
		}

		std::strong_ordering operator <=> (const ReflFunc& hook_context) {
			return reinterpret_cast<std::size_t>(hook_context._function_address) <=>
				reinterpret_cast<std::size_t>(_function_address);
		}
	};
}