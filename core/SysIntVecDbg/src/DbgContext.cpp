
#include<DbgExecuter.hpp>
using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;

bool DebugContext::getThreadContext(const ScopeHandle<>& hThread, DWORD contextFlags)
{
	_ctx.ContextFlags = contextFlags;
	if (GetThreadContext(hThread, &_ctx) == FALSE) {
		return false;
	}
	return true;
}
bool DebugContext::setThreadContext(const ScopeHandle<>& hThread) const
{
	if (SetThreadContext(hThread, &_ctx) == FALSE) {
		return false;
	}
	return true;
}

void DebugContext::recoverRegisterIP() {
#ifdef _WIN64
	_ctx.Rip = reinterpret_cast<DWORD64>(_fp_exception_address);
#else
	_ctx.Eip = reinterpret_cast<DWORD32>(_fp_exception_address);
#endif
}

auto DebugContext::recoverAndGetThreadContext()
	->ScopeHandle<>
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
	if (getThreadContext(dbg_thr) == false) {
		return nullptr;
	}
	return dbg_thr;
}
bool DebugContext::resumeThreadAndDebug(ScopeHandle<>&& threadScopeHandle)
{
	recoverRegisterIP();
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return false;
	}
	const auto& dbg_event = pDbgExecutor->_dbg_event;

	if (getThreadContext(threadScopeHandle) == false) {
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