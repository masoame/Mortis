#pragma once
#include<PE.h>
#include <ranges>  

namespace Mortis::PE
{
	template<typename FuncPtrType>
	struct HookContext : public ReflFunc<FuncPtrType>
	{
		INT_CODE _int_code;

		HookContext(FuncPtrType func_ptr) : 
			ReflFunc<FuncPtrType>(func_ptr) {}
	};
}