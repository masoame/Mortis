
#include<DbgExecuter.hpp>
using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


ScopeHandle<> DebugContext::recoverAndGetThreadContext()
{
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return nullptr;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_int_code, sizeof(BYTE), 0) == FALSE) {
		return nullptr;
	}
	ScopeHandle dbg_thr = OpenThreadHandle(pDbgExecutor->_dbg_event.dwThreadId);
	_ctx.ContextFlags = CONTEXT_CONTROL;
	if (GetThreadContext(dbg_thr, &_ctx) == FALSE) {
		return nullptr;
	}
	return dbg_thr;
}
bool DebugContext::resumeThreadAndDebug(ScopeHandle<>&& threadScopeHandle)
{
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return false;
	}
	const auto& dbg_event = pDbgExecutor->_dbg_event;

	if (SetThreadContext(threadScopeHandle, &_ctx) == FALSE) {
		return false;
	}
	if (ContinueDebugEvent(dbg_event.dwThreadId, dbg_event.dwThreadId, DBG_CONTINUE) == FALSE) {
		return false;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (WriteProcessMemory(hProcess, _fp_exception_address, &PE::INT3, sizeof(BYTE), NULL) == FALSE) {
		return false;
	}
	return true;
}