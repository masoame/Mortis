
#include<DbgExecuter.hpp>
using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


DebugContext::DebugContext(PVOID fp_exception_address, DWORD dw_exception_code, PE::INT_TYPE int_code)
{
	_fp_exception_address = fp_exception_address;
	_dw_exception_code = dw_exception_code;
	_int_code = int_code;
}

bool DebugContext::getThreadContext(const ScopeHandle<>& hThread, DWORD contextFlags)
{
	auto thread_ctx = _thread_ctx.lock();
	thread_ctx->ContextFlags = contextFlags;
	if (GetThreadContext(hThread, thread_ctx.get()) == FALSE) {
		return false;
	}
	return true;
}
bool DebugContext::setThreadContext(const ScopeHandle<>& hThread) const
{
	auto thread_ctx = _thread_ctx.lock();
	if (SetThreadContext(hThread, thread_ctx.get()) == FALSE) {
		return false;
	}
	return true;
}

void DebugContext::recoverRegisterRIP() noexcept {
	auto thread_ctx = _thread_ctx.lock();
#ifdef _WIN64
	thread_ctx->Rip = reinterpret_cast<DWORD64>(_fp_exception_address);
#else
	thread_ctx->Eip = reinterpret_cast<DWORD32>(_fp_exception_address);
#endif
}

bool DebugContext::bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept {
	if (_dbg_executer.expired()) {
		_dbg_executer = pDbgExecuter->weak_from_this();
		_thread_ctx = pDbgExecuter->_thread_ctx;
		return true;
	}
	return false;
}
bool DebugContext::startDebug() noexcept {
	auto pDbgExecutor = _dbg_executer.lock();
	if (!pDbgExecutor) {
		return false;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	return saveOrginCode(hProcess) && continueDebug(hProcess);
}

bool DebugContext::continueDebug(const ScopeHandle<>& hProcess) const noexcept {
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_int_code, sizeof(BYTE), nullptr) == FALSE) {
		return false;
	}
	return true;
}
bool DebugContext::stopDebug(const ScopeHandle<>& hProcess) const noexcept {
	if (WriteProcessMemory(hProcess, _fp_exception_address, &_origin_code, sizeof(BYTE), nullptr) == FALSE) {
		return false;
	}
	return true;
}

bool DebugContext::saveOrginCode(const ScopeHandle<>& hProcess) noexcept {
	if (ReadProcessMemory(hProcess, _fp_exception_address, &_origin_code, sizeof(BYTE), nullptr) == FALSE) {
		return false;
	}
	return true;
}

bool DebugContext::setIntCode(INT_TYPE int_code) noexcept {
	//if (INT_TABLE.contains(int_code)) {

	//}
	int_code;
	return false;
}

void DebugContext::regExceptionCallBack(std::function<void(DebugContext& )> callBackFunc)
{
	_call_exception_handler = std::bind(callBackFunc,std::ref(*this));
}

bool DebugContext::exceptionCallBack() const 
{
	try {
		_call_exception_handler();
	}
	catch (...) {
		return false;
	}
	return true;
}


auto DebugContext::refreshThreadContext()
	->ScopeHandle<>
{
	auto pDbgExecutor = _dbg_executer.lock();
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

	auto pDbgExecutor = _dbg_executer.lock();
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