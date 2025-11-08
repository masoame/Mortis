#pragma once
#include "utils.hpp"

namespace Mortis::Meta
{
	template<typename Object>
	struct MoBaseMate
	{
	public:
		constexpr std::string get_name() const {
			return static_cast<const Object*>(this)->name();
		}

		constexpr auto& get_fields() const {
			return static_cast<const Object*>(this)->fields();
		}

		constexpr auto& get_fields() {
			return static_cast<Object*>(this)->fields();
		}

		constexpr auto& get_fields_by_name(std::string_view name) const {
			return static_cast<const Object*>(this)->fields();
		}

		constexpr auto& get_fields_by_name(std::string_view name) {
			return static_cast<Object*>(this)->fields();
		}
	};

	template<typename Object>
	struct MoMeta
	{
		constexpr static std::string get_name(const MoBaseMate<Object>& refObjectMeta) {
			return refObjectMeta.get_name();
		}
	};
}

