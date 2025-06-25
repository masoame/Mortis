#include<DbgContext.hpp>

using namespace Mortis;
using namespace Mortis::SysIntVecDbg;


ScopeHandle<> DebugContext::beforeException()
{
	auto pDbgExecutor = _dbgExecuter.lock();

	if (WriteProcessMemory(pDbgExecutor->_th32ProcessID, lpfc, &code, sizeof(BYTE), 0) == FALSE) {
		return false;
	}

	ScopeHandle dbg_thr = OpenThread(THREAD_ALL_ACCESS, FALSE, pde.dwThreadId);
	CONTEXT ctx{};
	ctx.ContextFlags = CONTEXT_CONTROL;
	if (GetThreadContext(dbg_thr, &ctx) == FALSE)
		return false;
}
void DebugContext::afterException(ScopeHandle<> threadScopeHandle)
{
	//if (SetThreadContext(dbg_thr, &ctx) == FALSE)
	//	return false;

	//if (ContinueDebugEvent(pde.dwProcessId, pde.dwThreadId, DBG_CONTINUE) == FALSE)
	//	return false;

	//std::this_thread::sleep_for(std::chrono::milliseconds(1));

	//if (WriteProcessMemory(hProcess, (LPVOID)lpfc, &PE::INT3, sizeof(BYTE), NULL) == FALSE)
	//	return false;
}