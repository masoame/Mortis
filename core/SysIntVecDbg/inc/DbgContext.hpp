#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter;

	class DebugKey {
	public:
		DWORD _dw_exception_code = EXCEPTION_BREAKPOINT;
		PVOID _fp_exception_address;
		DebugKey() noexcept = default;
		auto operator<=>(const DebugKey&) const = default;
	};

	class DebugContext : public DebugKey {
		CONTEXT _ctx;
		BYTE _origin_code;
		BYTE _int_code;
		std::weak_ptr<DbgExecuter> _dbgExecuter;
	public:
		std::function<void()> _callExceptionHandler;

		bool getThreadContext(const ScopeHandle<>& hThread,DWORD contextFlags = CONTEXT_ALL);
		bool setThreadContext(const ScopeHandle<>& hThread) const;
		void recoverRegisterIP() noexcept;

		auto recoverAndGetThreadContext() 
			-> ScopeHandle<>;
		bool resumeThreadAndDebug(ScopeHandle<>&& threadHandle);
	};
}
