#pragma once
#include<sstream>
#include <utils.hpp>
#include <asm_types.hpp>
#include <PE_template.hpp>

namespace Mortis::PE
{
	//打开id对应的进程句柄
	template<typename HasIdType>
	auto OpenProcessHandle(const HasIdType& hasIdType, DWORD dwDesiredAccess = PROCESS_ALL_ACCESS, BOOL bInheritHandle = FALSE)
		-> ScopeHandle<>;

	auto OpenThreadHandle(DWORD dwThreadId,DWORD dwDesiredAccess = THREAD_ALL_ACCESS,BOOL bInheritHandle = FALSE)
		-> ScopeHandle<>;

	//搜索进程
	template<typename Type>
	auto SearchProcess(std::basic_string_view<Type> process_name_view)
		-> std::unique_ptr<PROCESSENTRY32<Type>>;

	//搜索模块
	template<typename Type>
	auto SearchModule(DWORD th32ProcessID, std::basic_string_view<Type> module_name_view)
		-> std::unique_ptr<MODULEENTRY32<Type>>;

	//进程信息
	template<typename T>
	auto ProcessInfo()
		-> std::vector<PROCESSENTRY32<T>>;

	//模块信息
	template<typename T>
	auto ModuleInfo(DWORD th32ProcessID)
		-> std::vector<MODULEENTRY32<T>>;

	//获得DOS头和NT头
	auto GetFileHeader(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::unique_ptr<std::pair<IMAGE_DOS_HEADER, IMAGE_NT_HEADERS>>;
}

#include<PE.hpp>
#include<ExportTable.hpp>
#include<ImportTable.hpp>