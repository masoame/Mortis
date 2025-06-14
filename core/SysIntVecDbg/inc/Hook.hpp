#pragma once

#include <PE.hpp>


namespace Mortis::SysIntVecDbg
{

	//勾取相应函数
	bool HookPrc(DWORD th32ProcessID, HMODULE hModule, const char* HookFunction, const std::function<void()>& OnHooked);

	//注入dll(dll路径与inject_dll.exe在同一级)
	DWORD InjectDll(DWORD th32ProcessID, const wchar_t* dll);

	//auto SetBreakPoint(const HANDLE hProcess, const HMODULE hModule, LPVOID const lpfc)
	//	-> std::pair<LPVOID, BYTE>;

	//extern std::atomic_bool IsHooking;
	//extern std::map<LPCVOID,BYTE> BreakPoints;
}