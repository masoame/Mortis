#pragma once

#include<HookContext.h>

namespace Mortis::PE
{
	class HookExecutor
	{
	public:
		HookExecutor();
		~HookExecutor();
	protected:

		template<typename FuncPtrType>
		inline void RegHook(HookContext<FuncPtrType> ctx)
		{
			auto A = ctx.refl_args_positions_map();
		}
	};





}