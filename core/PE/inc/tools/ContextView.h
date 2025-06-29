#pragma once

#include <ReflFuncResult.h>
#include <any>

namespace Mortis::PE
{
	enum CallConventions {
		STDCALL_X64,
		CDECL_X64,
		FASTCALL_X64,

		STDCALL_X86,
		CDECL_X86,
		FASTCALL_X86,
	};


	class ContextView
	{
	private:
		//ReflFuncResult<void()> _reflFuncResult;
		const CONTEXT& _context;
	public:
		ContextView(const CONTEXT& context) : _context(context) {}

		auto getArg(std::size_t location, std::function<std::any(DWORD64)> callToConvertType, CallConventions callconventions = STDCALL_X64)
			-> std::any;
	};
}