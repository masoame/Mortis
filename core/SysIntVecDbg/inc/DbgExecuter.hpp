#pragma once
#include<PE.h>
#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter : std::enable_shared_from_this<DbgExecuter> {
		friend class DebugContext;

		std::jthread _dbg_thread;
		std::map<DebugKey, DebugContext> _dbg_contexts;
	protected:
		DWORD _th32ProcessID;
		HMODULE _hModule;

		void dbgThrTemplate(std::stop_token st);
		BOOL DebugEventExector();
	public:
		bool regDbgContext(std::string_view HookFunction, const std::function<void()>& OnHooked);
		DbgExecuter(DWORD th32ProcessID, HMODULE hModule);
		~DbgExecuter() {
			_dbg_thread.request_stop();
		}
	};
}