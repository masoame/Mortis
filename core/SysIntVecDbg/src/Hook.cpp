#include <utils.hpp>

#include"asm_types.hpp"
#include "Hook.hpp"


#include "ExportTable.hpp"

#include<windows.h>
#include<iostream>
#include<sstream>
#include<filesystem>
#include<thread>
#include<set>
#include<map>
#include<future>

namespace Mortis::SysIntVecDbg {

	DWORD InjectDll(DWORD th32ProcessID, const wchar_t* dll)
	{
		//通过相应进程id打开对应进程
		ScopeHandle<> ph = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, th32ProcessID);
		if (ph) {
			std::cout << "成功打开对应进程" << std::endl;
		}else {
			return (DWORD)-1;
		}
		//文件路径
		std::wstring location = std::filesystem::path(os::GetModuleFileNameW().value()).replace_filename(dll);

		//为对应内存申请进程，返回申请内存的地址
		LPVOID ptrmem = VirtualAllocEx(ph, nullptr, (location.size() + 1) * sizeof(wchar_t), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ptrmem)std::cout << "对应进程申请内存成功" << std::endl;
		else return (DWORD)-1;

		//写入对应内存dll路径
		auto temp = WriteProcessMemory(ph, ptrmem, location.data(), (location.size() + 1) * sizeof(wchar_t), 0);
		if (temp)std::cout << "写入对应内存成功" << std::endl;
		else return (DWORD)-1;

		//注入线程
		ScopeHandle<> target_thread = ::CreateRemoteThread(ph, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryW, ptrmem, 0, 0);
		if (target_thread == false)return (DWORD)-1;

		temp = ::WaitForSingleObject(target_thread, INFINITE);
		if (temp != -1)std::cout << "注入线程执行完毕" << std::endl;
		else return (DWORD)-1;

		//释放申请的内存
		temp = ::VirtualFreeEx(ph, ptrmem, 0, MEM_RELEASE);
		if (temp)std::cout << "内存释放成功" << std::endl;
		else return (DWORD)-1;

		return (DWORD)0;
	}


	std::optional<std::pair<PE::FunctionPtr, BYTE>> SetBreakPoint(const HANDLE hProcess, const LPVOID lpfc)
	{
		if (lpfc == nullptr || hProcess == nullptr)
			return std::nullopt;

		BYTE code = 0;
		if (ReadProcessMemory(hProcess, lpfc, &code, sizeof(BYTE), NULL) == FALSE)
			return std::nullopt;
		if (WriteProcessMemory(hProcess, lpfc, &PE::INT3, sizeof(BYTE), NULL) == FALSE)
			return std::nullopt;

		return { { lpfc , code } };
	}

	std::map<LPVOID, BYTE> SetBreakPoint(const HANDLE hProcess, const std::vector<PE::FunctionPtr>& lpfcs)
	{
		std::map<PE::FunctionPtr, BYTE> dbg_tb;
		for (const auto& lpfc : lpfcs) {
			if (auto opt = SetBreakPoint(hProcess, lpfc); opt.has_value()) {
				dbg_tb.insert(opt.value());
			}
		}
		return dbg_tb;
	}


	std::map<LPVOID, BYTE> SetBreakPoint(const HANDLE hProcess, const HMODULE hModule, const std::vector<std::string_view>& func_names)
	{
		std::map<LPVOID, BYTE> dbg_tb;
		auto lpfcs = PE::Exp::GetProcAddressEx(hProcess, hModule, func_names);
		return SetBreakPoint(hProcess, lpfcs);
	}

	//接收到中断信号后
	bool OnExceptionDebugEvent(const HANDLE hProcess, const DEBUG_EVENT& pde, const FARPROC lpfc, const BYTE& code, const std::function<void()>& OnHooked)
	{
		//处理器的寄存器数据相关结构体
		const auto& exception_record = pde.u.Exception.ExceptionRecord;

		if (exception_record.ExceptionCode == EXCEPTION_BREAKPOINT) {
			if (exception_record.ExceptionAddress == lpfc) {

				if (WriteProcessMemory(hProcess, lpfc, &code, sizeof(BYTE), 0) == FALSE)
					return false;

				ScopeHandle dbg_thr = OpenThread(THREAD_ALL_ACCESS, FALSE, pde.dwThreadId);
				CONTEXT ctx{};
				ctx.ContextFlags = CONTEXT_CONTROL;
				if (GetThreadContext(dbg_thr, &ctx) == FALSE)
					return false;

				OnHooked();

#ifdef _WIN64
				ctx.Rip = reinterpret_cast<DWORD64>(lpfc);
#else
				ctx.Eip = reinterpret_cast<DWORD32>(lpfc);
#endif


				if (SetThreadContext(dbg_thr, &ctx) == FALSE)
					return false;

				if (ContinueDebugEvent(pde.dwProcessId, pde.dwThreadId, DBG_CONTINUE) == FALSE)
					return false;

				std::this_thread::sleep_for(std::chrono::milliseconds(1));

				if (WriteProcessMemory(hProcess, (LPVOID)lpfc, &PE::INT3, sizeof(BYTE), NULL) == FALSE)
					return false;
				return true;
			}
		}
		return false;
	}

	bool HookPrc(DWORD th32ProcessID, HMODULE hModule, const char* HookFunction, const std::function<void()>& OnHooked)
	{
		if (DebugActiveProcess(th32ProcessID) == FALSE)
			return false;

		PE::FunctionPtr lpfc = 0;
		BYTE code = 0;
		DEBUG_EVENT de;
		DWORD dcstatus;

		CREATE_PROCESS_DEBUG_INFO pinfo{};

		while (WaitForDebugEvent(&de, INFINITE))
		{
			dcstatus = DBG_CONTINUE;
			if (de.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
				pinfo = de.u.CreateProcessInfo;

				lpfc = PE::Exp::GetProcAddressEx(pinfo.hProcess, hModule, HookFunction);

				if (ReadProcessMemory(pinfo.hProcess, lpfc, &code, sizeof(BYTE), NULL) == FALSE)
					return false;
				if (WriteProcessMemory(pinfo.hProcess, lpfc, &PE::INT3, sizeof(BYTE), NULL) == FALSE)
					return false;
			}
			else if (de.dwDebugEventCode == EXCEPTION_DEBUG_EVENT) {
				if (OnExceptionDebugEvent(pinfo.hProcess, de, reinterpret_cast<FARPROC>(lpfc), code, OnHooked))
					continue;
			}
			else if (de.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
				break;
			}

			if (ContinueDebugEvent(de.dwProcessId, de.dwThreadId, dcstatus) == FALSE)
				return false;
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return true;
	}
}