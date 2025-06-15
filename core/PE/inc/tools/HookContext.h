#pragma once
#include<PE.h>
#include <ranges>  
namespace Mortis::PE
{
	template<typename FuncPtrType>
	struct HookContext : public ReflFunc<FuncPtrType>
	{
		HookContext(FuncPtrType func_ptr) : 
			ReflFunc<FuncPtrType>(func_ptr) {}
	};



}