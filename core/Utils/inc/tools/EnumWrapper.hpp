#pragma once
#include <utils.hpp>

namespace Mortis
{
	template<typename T>
	concept ConEnum = std::is_enum_v<T>;

	template<typename T>
	concept ConEnumFlag = std::is_enum_v<T>;

	template<ConEnumFlag T>
	class FlagView
	{
	protected:
		using Interger = std::underlying_type_t<T>;
		const Interger& _flags;
	public:
		FlagView(const Interger& flags) : _flags(flags) {}

		constexpr bool contains(const Interger& flags) const {
			return _flags & flags;
		}

		constexpr bool is_inc(const Interger& flags) const {
			return (_flags & flags) == flags;
		}

		constexpr bool is_same(const Interger& flags) const {
			return _flags == flags;
		}

		constexpr const Interger& get() const {
			return _flags;
		}
	};

	template<typename T>
	class FlagEdit : public FlagView<T> 
	{
		FlagView<T>::Interger _flags;
	public:
		FlagEdit() : FlagView(_flags), _flags(0) {}
		FlagEdit(const FlagView<T>::Interger& flags) : FlagView(_flags),_flags(flags) {}

		constexpr void erase(const FlagView<T>::Interger& flags) {
			_flags &= ~flags;
		}

		constexpr void add(const FlagView<T>::Interger& flags) {
			_flags |= flags;
		}
	};
}

