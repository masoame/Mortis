#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	extern class DbgExecuter;

	class DebugKey {
	public:
		DWORD _dw_exception_code;
		PVOID _fp_exception_address;
		auto operator<=>(const DebugKey&) const = default;
	};

	class DebugContext : public DebugKey {
		CONTEXT _ctx;
	public:
		BYTE _origin_code;
		BYTE _int_code;

		std::function<void()> _callExceptionHandler;

		std::weak_ptr<DbgExecuter> _dbgExecuter;
		ScopeHandle<> beforeException();
		void afterException(ScopeHandle<> threadHandle);
	};
}
