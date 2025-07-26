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
		friend struct DbgContext;
	private:
		DEBUG_EVENT _dbg_event;
		std::jthread _dbg_thread;
		std::map<DbgKey, std::unique_ptr<DbgContext>> _dbg_contexts;
		std::shared_ptr<CONTEXT> _thread_ctx;
		DWORD _th32ProcessID;
		void dbgThrMain(std::stop_token st);
	public:
		explicit DbgExecuter(DWORD th32ProcessID);
		~DbgExecuter();

		bool regDbgContext(std::unique_ptr<DbgContext>&& dbgContext);
		void wait();
	};
}