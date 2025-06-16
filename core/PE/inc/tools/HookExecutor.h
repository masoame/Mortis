#pragma once

#include<HookContext.h>

namespace Mortis::PE
{
	class HookExecutor
	{
		HookExecutor();

		template<typename FuncPtrType>
		inline void HookExecutor::RegHook(HookContext<FuncPtrType> ctx)
		{
			auto A = ctx.refl_args_positions_map();
		}
	};





}