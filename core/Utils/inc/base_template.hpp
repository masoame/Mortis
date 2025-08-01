#pragma once
#include <type_traits>
#include <memory>
namespace Mortis::BT
{
	template <auto F>
	using StaticFunctorWrapper = std::integral_constant<std::decay_t<decltype(F)>, F>;

    template<typename T, typename... Args>
    constexpr auto get_type_positions_impl() {
        constexpr std::size_t count = (std::is_same_v<T, Args> +...);
        std::array<std::size_t, count> result{};
        std::size_t index = 0;

        [&] <std::size_t... Is>(std::index_sequence<Is...>) {
            ((std::is_same_v<T, Args> ? (void)(result[index++] = Is) : void()), ...);
        }(std::make_index_sequence<sizeof...(Args)>{});

        return result;
    }

    template<typename T, typename R, typename... Args>
    constexpr auto get_type_positions(R(*)(Args...)) {
        return get_type_positions_impl<T, Args...>();
    }

    template<typename T, typename C, typename R, typename... Args>
    constexpr auto get_type_positions(R(C::*)(Args...)) {
        return get_type_positions_impl<T, Args...>();
    }

    template<typename T, typename C, typename R, typename... Args>
    constexpr auto get_type_positions(R(C::*)(Args...) noexcept) {
        return get_type_positions_impl<T, Args...>();
    }

    template<typename T, typename C, typename R, typename... Args>
    constexpr auto get_type_positions(R(C::*)(Args...) const) {
        return get_type_positions_impl<T, Args...>();
    }

    template<typename T, typename C, typename R, typename... Args>
    constexpr auto get_type_positions(R(C::*)(Args...) const noexcept) {
        return get_type_positions_impl<T, Args...>();
    }
}