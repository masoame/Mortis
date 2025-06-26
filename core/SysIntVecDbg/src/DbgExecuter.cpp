#include<DbgExecuter.hpp>

using namespace Mortis::SysIntVecDbg;

DbgExecuter::DbgExecuter(DWORD th32ProcessID, HMODULE hModule) :
	_th32ProcessID(th32ProcessID),
	_hModule(hModule),
	_dbg_thread(std::bind(&DbgExecuter::dbgThrTemplate, this,std::placeholders::_1))
{ }

void DbgExecuter::dbgThrTemplate(std::stop_token st)
{
	if (DebugActiveProcess(_th32ProcessID) == FALSE) {
		return;
	}
	ScopeExecutor closeExecutor([_this = shared_from_this()] {
		DebugActiveProcessStop(_this->_th32ProcessID);
	});
	DWORD dcstatus;
	const auto& exception_record = _dbg_event.u.Exception.ExceptionRecord;

	while (WaitForDebugEvent(&_dbg_event, INFINITE) && st.stop_requested())
	{
		dcstatus = DBG_CONTINUE;
		if (_dbg_event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {

			DebugKey debugKey{
				._dw_exception_code = exception_record.ExceptionCode,
				._fp_exception_address = exception_record.ExceptionAddress
			};
			if (_dbg_contexts.contains(debugKey)) {
				_dbg_contexts[debugKey]._callExceptionHandler();
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

		if (ContinueDebugEvent(_dbg_event.dwProcessId, _dbg_event.dwThreadId, dcstatus) == FALSE) {
			return;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return;
}
//bool DbgExecuter::DebugEventExector(const HANDLE hProcess, const DEBUG_EVENT& pde, const FARPROC lpfc, const BYTE& code, const std::function<void()>& OnHooked)
//{
//	//处理器的寄存器数据相关结构体
//	const auto& exception_record = pde.u.Exception.ExceptionRecord;
//
//	if (exception_record.ExceptionCode == EXCEPTION_BREAKPOINT) {
//		if (exception_record.ExceptionAddress == lpfc) {
//
//			if (WriteProcessMemory(hProcess, lpfc, &code, sizeof(BYTE), 0) == FALSE)
//				return false;
//
//			ScopeHandle dbg_thr = OpenThread(THREAD_ALL_ACCESS, FALSE, pde.dwThreadId);
//			CONTEXT ctx{};
//			ctx.ContextFlags = CONTEXT_CONTROL;
//			if (GetThreadContext(dbg_thr, &ctx) == FALSE)
//				return false;
//
//			OnHooked();
//
//#ifdef _WIN64
//			ctx.Rip = reinterpret_cast<DWORD64>(lpfc);
//#else
//			ctx.Eip = reinterpret_cast<DWORD32>(lpfc);
//#endif
//
//
//			if (SetThreadContext(dbg_thr, &ctx) == FALSE)
//				return false;
//
//			if (ContinueDebugEvent(pde.dwProcessId, pde.dwThreadId, DBG_CONTINUE) == FALSE)
//				return false;
//
//			std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//			if (WriteProcessMemory(hProcess, (LPVOID)lpfc, &PE::INT3, sizeof(BYTE), NULL) == FALSE)
//				return false;
//			return true;
//		}
//	}
//	return false;
//}
bool DbgExecuter::regDbgContext(std::string_view HookFunction, const std::function<void()>& OnHooked)
{
	HookFunction, OnHooked;
	return true;
}