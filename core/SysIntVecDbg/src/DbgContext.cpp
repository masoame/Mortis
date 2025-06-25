
#include<DbgExecuter.hpp>
using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


ScopeHandle<> DebugContext::beforeException()
{
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return nullptr;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_int_code, sizeof(BYTE), 0) == FALSE) {
		return nullptr;
	}

	ScopeHandle dbg_thr = OpenThread(THREAD_ALL_ACCESS, FALSE, pde.dwThreadId);

	_ctx.ContextFlags = CONTEXT_CONTROL;
	if (GetThreadContext(dbg_thr, &_ctx) == FALSE) {
		return nullptr;
	}
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