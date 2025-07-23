#include<DbgExecuter.hpp>

using namespace Mortis::SysIntVecDbg;

DbgExecuter::DbgExecuter(DWORD th32ProcessID) :
	_thread_ctx(new CONTEXT{}),
	_th32ProcessID(th32ProcessID),
	_dbg_thread(std::bind(&DbgExecuter::dbgThrMain, this, std::placeholders::_1))
{ }

DbgExecuter::~DbgExecuter() {
	_dbg_thread.request_stop();
}

void DbgExecuter::wait() {
	_dbg_thread.join();
}

void DbgExecuter::dbgThrMain(std::stop_token st)
{
	if (DebugActiveProcess(_th32ProcessID) == FALSE) {
		return;
	}
	ScopeExecutor closeExecutor([_this = shared_from_this()] {
		DebugActiveProcessStop(_this->_th32ProcessID);
	});
	const auto& exception_record = _dbg_event.u.Exception.ExceptionRecord;

	while (WaitForDebugEvent(&_dbg_event, INFINITE) && (st.stop_requested() == false))
	{
		if (_dbg_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {

			DebugKey debugKey{
				._dw_exception_code = exception_record.ExceptionCode,
				._fp_exception_address = exception_record.ExceptionAddress
			};
			if (_dbg_contexts.contains(debugKey)) {

				const auto& ctx = _dbg_contexts[debugKey];
				auto hDbgThread = ctx->refreshThreadContext();
                ScopeExecutor resumeThread{ [&ctx, hThread = std::move(hDbgThread)]() mutable {
                   if (ctx->applyThreadContext(std::move(hThread)) == false) {
                       spdlog::error(std::format("{}:{} error!!!", __FILE__, __LINE__));
                   }} 
				};
				if (ctx->exceptionCallBack()) {
					continue;
				}
			}
		}
		else if (_dbg_event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
		
		}
		else if (_dbg_event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
			break;
		}
		else if (_dbg_event.dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT) {

		}
		else if (_dbg_event.dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT) {

		}

		if (ContinueDebugEvent(_dbg_event.dwProcessId, _dbg_event.dwThreadId, DBG_CONTINUE) == FALSE) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return;
}
bool DbgExecuter::regDbgContext(std::unique_ptr<DebugContext>&& dbgContext)
{
	if (_dbg_contexts.contains(*dbgContext) == false && 
			dbgContext->bindDbgExecuter(shared_from_this()) && 
				dbgContext->startDebug()) {

		_dbg_contexts.emplace(std::make_pair(*dbgContext,std::move(dbgContext)));
		return true;
	}
	return false;
}