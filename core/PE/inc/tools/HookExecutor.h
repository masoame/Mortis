#pragma once

#include<HookContext.h>

namespace Mortis::PE
{
	class HookExecutor
	{
		HookExecutor();
		~HookExecutor();
	public:

		HookExecutor* Instance() {
			static HookExecutor instance;
			return &instance;
		}

		template<typename FuncPtrType>
		inline void RegHook(HookContext<FuncPtrType> ctx){
			auto A = ctx.refl_args_positions_map();
		}
	};





}