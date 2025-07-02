#pragma once
#include<PE.h>
#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgReflStore
	{
		DWORD _th32ProcessID;
		DbgReflStore(DWORD th32ProcessID);
	};


	class DbgExecuter : public std::enable_shared_from_this<DbgExecuter> 
	{
		friend struct DebugContext;
		DEBUG_EVENT _dbg_event;

		std::jthread _dbg_thread;
		std::map<DebugKey, std::unique_ptr<DebugContext>> _dbg_contexts;
	protected:
		DWORD _th32ProcessID;

		void dbgThrMain(std::stop_token st);
		BOOL DebugEventExector();
	public:
		explicit DbgExecuter(DWORD th32ProcessID);
		~DbgExecuter();

		bool regDbgContext(std::unique_ptr<DebugContext>&& dbgContext);
		void wait();
	};
}