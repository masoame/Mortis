#pragma once
#include <type_traits>
#include <memory>
namespace Mortis::BT
{
	template <auto F>
	using StaticFunctorWrapper = std::integral_constant<std::remove_reference_t<decltype(F)>, F>;

	template<typename HandleType, typename DeleteFunc>
	struct ScopeHandleWrapper 
	{
		using Type = std::remove_reference_t<HandleType>;
		constexpr static bool isPtr = std::is_pointer_v<Type>;
		using UniqueType = std::conditional_t<isPtr, std::remove_pointer_t<Type>, Type>;

		using Ptr = UniqueType*;
		using SecPtr = Ptr*;

		constexpr static bool isSecPtr = not std::is_invocable_v<typename DeleteFunc::value_type, Ptr>;

		struct DeletePrimaryPtr {
			void operator()(void* ptr) {
				DeleteFunc()(static_cast<Ptr>(ptr));
			}
		};
		struct DeleteSecPtr {
			void operator()(void* ptr) {
				DeleteFunc()(reinterpret_cast<SecPtr>(&ptr));
			}
		};

		using DeletePtr = std::conditional_t<isSecPtr, DeleteSecPtr, DeletePrimaryPtr >;

		using unique_ptr = std::unique_ptr<UniqueType, DeletePtr>;
	};
}