#include<PE.h>

namespace Mortis::PE
{
	struct HookContext
	{
		FuncPtr _function_address = nullptr;

		std::strong_ordering operator <=> (const HookContext& hook_context) {
			return hook_context._function_address <=> _function_address;
		}
	};
}