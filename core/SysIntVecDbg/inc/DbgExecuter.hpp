#pragma once
#include<PE.h>
#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter : public std::enable_shared_from_this<DbgExecuter> {
		friend class DebugContext;

		std::jthread _dbg_thread;
		std::map<DebugKey, ScopeHandle<DebugContext>> _dbg_contexts;

		DEBUG_EVENT _dbg_event;
	protected:
		DWORD _th32ProcessID;

		void dbgThrTemplate(std::stop_token st);
		BOOL DebugEventExector();
	public:

		bool regDbgContext(ScopeHandle<DebugContext>&& debugContext);
		DbgExecuter(DWORD th32ProcessID);
		~DbgExecuter() {
			_dbg_thread.request_stop();
		}
	};
}