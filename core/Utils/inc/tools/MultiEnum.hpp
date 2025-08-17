#pragma once
#include<utils.hpp>

namespace Mortis {

	template<typename EnumType>
	class MultiEnum
	{
		static_assert(std::is_enum_v<EnumType>);
		EnumType _value;
	public:
		MultiEnum() = default;
		MultiEnum(const MultiEnum&) = default;

		MultiEnum(EnumType value = EnumType())
			: _value(value)
		{
		}

		operator EnumType() const {
			return _value;
		}

		MultiEnum& operator |=(const EnumType& other) {
			_value |= other;
			return *this;
		}

		bool has_value(const EnumType& other) const {
			return (_value & other) == other;
		}
	};
}

