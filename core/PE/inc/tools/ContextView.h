#pragma once

#include <ReflFuncResult.h>
#include <any>

namespace Mortis::PE
{
	template<typename FuncType>
	class ContextView
	{
	private:
		const CONTEXT& _context;
	public:
		ContextView(const CONTEXT& context) : _context(context) {}
		
		DWORD sp();

		operator const CONTEXT& () const {
			return _context;
		}

		auto getArg(std::size_t location, CallingConvention callconventions = X64_CALL);
	};
}