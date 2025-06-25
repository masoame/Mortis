#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	extern class DbgExecuter;

	class DebugKey {
	public:
		DWORD _nExceptionCode;
		PVOID _fpExceptionAddress;
		auto operator<=>(const DebugKey&) const = default;
	};

	class DebugContext : public DebugKey {
	public:
		BYTE _orginCode;
		BYTE _intCode;

		std::function<void()> _callExceptionHandler;

		std::weak_ptr<DbgExecuter> _dbgExecuter;
		ScopeHandle<> beforeException();
		void afterException(ScopeHandle<> threadHandle);
	};
}
