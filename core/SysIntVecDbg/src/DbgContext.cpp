
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

void DebugContext::recoverRegisterRIP() noexcept {
#ifdef _WIN64
	_ctx.Rip = reinterpret_cast<DWORD64>(_fp_exception_address);
#else
	_ctx.Eip = reinterpret_cast<DWORD32>(_fp_exception_address);
#endif
}

bool DebugContext::bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept {
	if (_dbgExecuter.expired()) {
		_dbgExecuter = pDbgExecuter->weak_from_this();
		return true;
	}
	return false;
}
bool DebugContext::startDebug() noexcept {
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return false;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	return saveOrginCode(hProcess) && continueDebug(hProcess);
}

bool DebugContext::continueDebug(const ScopeHandle<>& hProcess) const noexcept {
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_int_code, sizeof(BYTE), NULL) == FALSE) {
		return false;
	}
	return true;
}
bool DebugContext::stopDebug(const ScopeHandle<>& hProcess) const noexcept {
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_origin_code, sizeof(BYTE), 0) == FALSE) {
		return false;
	}
	return true;
}

bool DebugContext::saveOrginCode(const ScopeHandle<>& hProcess) noexcept {
	if (ReadProcessMemory(hProcess, _fp_exception_address, &_origin_code, sizeof(BYTE), NULL) == FALSE) {
		return false;
	}
	return true;
}

bool DebugContext::setIntCode(INT_TYPE int_code) noexcept {
	if (INT_TABLE.contains(int_code)) {

	}
	return false;
}

void DebugContext::regExceptionCallBack(std::function<void()> callBackFunc)
{
	_callExceptionHandler = std::bind(callBackFunc);
}

bool DebugContext::exceptionCallBack() const 
{
	try {
		_callExceptionHandler();
	}
	catch (...) {
		return false;
	}
	return true;
}


auto DebugContext::refreshThreadContext()
	->ScopeHandle<>
{
	auto pDbgExecutor = _dbgExecuter.lock();
	if (!pDbgExecutor) {
		return nullptr;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (stopDebug(hProcess) == false) {
		return nullptr;
	}

	ScopeHandle dbg_thr = OpenThreadHandle(pDbgExecutor->_dbg_event.dwThreadId);
	if (getThreadContext(dbg_thr) == false) {
		return nullptr;
	}
	return dbg_thr;
}
bool DebugContext::applyThreadContext(ScopeHandle<>&& threadScopeHandle)
{
	recoverRegisterRIP();

	auto pDbgExecutor = _dbgExecuter.lock();
	if (pDbgExecutor == nullptr) {
		return false;
	}
	const auto& dbg_event = pDbgExecutor->_dbg_event;

	if (setThreadContext(threadScopeHandle) == false) {
		return false;
	}
	if (ContinueDebugEvent(dbg_event.dwProcessId, dbg_event.dwThreadId, DBG_CONTINUE) == FALSE) {
		return false;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (continueDebug(hProcess) == false) {
		return false;
	}
	return true;
}