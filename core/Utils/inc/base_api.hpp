#pragma once
#include<type_traits>
#include<base_template.hpp>
namespace Mortis
{
/*
* reverse_bit用于反转位域reverse_bit
*/
	template<class _T1>
	constexpr void reverse_bit(_T1& val, _T1 local) noexcept requires std::is_pod_v<_T1>
	{
		val = (val & local) ? (val & (~local)) : (val | local);
	}
}
