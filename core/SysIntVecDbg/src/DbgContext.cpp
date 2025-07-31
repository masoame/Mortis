
#include<DbgExecuter.hpp>
using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


DbgContext::DbgContext(PVOID fp_exception_address, DWORD dw_exception_code, std::span<const BYTE> replace_code) :
	DbgKey{
		._dw_exception_code = dw_exception_code,
		._fp_exception_address = fp_exception_address
	},
	_replace_code(replace_code.begin(),replace_code.cend()),
	_origin_code(0)
{ }

DbgContext::DbgContext(PVOID fp_exception_address, std::span<const BYTE> replace_code):
	DbgContext(fp_exception_address,0, replace_code)
{ }

std::shared_ptr<DbgExecuter> DbgContext::executor() const
{
	return _dbg_executer.lock();
}
std::shared_ptr<CONTEXT> DbgContext::ctx() const
{
	return _thread_ctx.lock();
}

std::expected<std::shared_ptr<DbgExecuter>, std::string_view> DbgContext::try_executor() const
{
	auto dbg_executer = executor();
	if (dbg_executer == nullptr) {
		return std::unexpected("no bind Executer");
	}
	return dbg_executer;
}

std::expected<std::shared_ptr<CONTEXT>,std::string_view> DbgContext::try_ctx() const
{
	auto thread_ctx = ctx();
	if (thread_ctx == nullptr) {
		return std::unexpected("no bind context");
	}
	return thread_ctx;
}


std::expected<std::shared_ptr<CONTEXT>, std::string_view> DbgContext::getThreadContext(const ScopeHandle<>& hThread, DWORD contextFlags)
{
	return try_ctx()
		.and_then(
			[&](std::shared_ptr<CONTEXT> ctx){
				ctx->ContextFlags = contextFlags;
				return GetThreadContext(hThread, ctx.get()) ? 
					std::expected<std::shared_ptr<CONTEXT>,std::string_view>(ctx) :
					std::unexpected("failed to getThreadContext!!!");
			});
}
bool DbgContext::setThreadContext(const ScopeHandle<>& hThread, std::shared_ptr<CONTEXT> ctx) const
{
	const auto thread_ctx = try_ctx().value_or(ctx);
	return SetThreadContext(hThread, thread_ctx.get()) != FALSE;
}

void DbgContext::recoverRegisterRIP() noexcept {
	auto thread_ctx = _thread_ctx.lock();
	if (thread_ctx == nullptr) {
		return;
	}
#ifdef _WIN64
	thread_ctx->Rip = reinterpret_cast<DWORD64>(_fp_exception_address);
#else
	thread_ctx->Eip = reinterpret_cast<DWORD32>(_fp_exception_address);
#endif
}

bool DbgContext::bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept {
	if (_dbg_executer.expired()) {
		_dbg_executer = pDbgExecuter->weak_from_this();
		_thread_ctx = pDbgExecuter->_thread_ctx;
		return true;
	}
	return false;
}
bool DbgContext::startDebug() noexcept {
	auto pDbgExecutor = _dbg_executer.lock();
	if (!pDbgExecutor) {
		return false;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	return saveOrginCode(hProcess) && continueDebug(hProcess);
}

bool DbgContext::continueDebug(const ScopeHandle<>& hProcess) const noexcept {
	return WriteProcessMemory(hProcess, _fp_exception_address, _replace_code.data(), _replace_code.size(), nullptr) != FALSE;
}
bool DbgContext::stopDebug(const ScopeHandle<>& hProcess) const noexcept {
	return WriteProcessMemory(hProcess, _fp_exception_address, _origin_code.data(), _origin_code.size(), nullptr) != FALSE;
}

bool DbgContext::saveOrginCode(const ScopeHandle<>& hProcess) noexcept {
	_origin_code.resize(_replace_code.size());
	return ReadProcessMemory(hProcess, _fp_exception_address, _origin_code.data(), _origin_code.size(), nullptr) != FALSE;
}

bool DbgContext::setCode(std::span<const BYTE> replace_code) noexcept {
	_replace_code.resize(replace_code.size());
	std::copy(replace_code.begin(), replace_code.end(), _replace_code.begin());
	return false;
}

void DbgContext::regExceptionCallBack(std::function<void(DbgContext& )> callBackFunc)
{
	_call_exception_handler = std::bind(callBackFunc,std::ref(*this));
}

bool DbgContext::exceptionCallBack() 
{
	try {
		_call_exception_handler();
	}
	catch (...) {
		return false;
	}
	return true;
}


auto DbgContext::refreshThreadContext()
	->ScopeHandle<>
{
	auto pDbgExecutor = executor();
	if (!pDbgExecutor) {
		return nullptr;
	}
	const auto hProcess = OpenProcessHandle(pDbgExecutor->_th32ProcessID);
	if (stopDebug(hProcess) == false) {
		return nullptr;
	}

	ScopeHandle dbg_thr = OpenThreadHandle(pDbgExecutor->_dbg_event.dwThreadId);

	if (getThreadContext(dbg_thr).has_value() == false) {
		return nullptr;
	}
	return dbg_thr;
}
bool DbgContext::applyThreadContext(ScopeHandle<>&& threadScopeHandle)
{
	recoverRegisterRIP();

	auto pDbgExecutor = executor();
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