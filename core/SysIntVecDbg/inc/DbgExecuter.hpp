#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter {
		std::jthread _dbg_thread;


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