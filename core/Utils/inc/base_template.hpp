#pragma once
#include <type_traits>
#include <memory>
namespace Mortis::BT
{
	template <auto F>
	using StaticFunctorWrapper = std::integral_constant<std::remove_reference_t<decltype(F)>, F>;
}