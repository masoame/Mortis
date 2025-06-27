#pragma once
#include<PE.h>

namespace Mortis::PE
{
	class MetaRelfOfFuncString {
		std::size_t _offset_args;
		std::size_t _string_len;
		CodePage _code_page;

		auto operator <=>(const MetaRelfOfFuncString& ) const = default;
	};


	template<typename FuncPtrType>
	struct HookContext : public ReflFunc<FuncPtrType>
	{
		std::set<MetaRelfOfFuncString> _meta_refl_of_func_strings{};

		HookContext() = default;
		HookContext(FuncPtrType func_ptr) : 
			ReflFunc<FuncPtrType>(func_ptr) {

		}
	};
}