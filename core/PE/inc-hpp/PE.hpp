#pragma once
#include<PE.h>

namespace Mortis::PE
{
	template<typename HasIdType>
	auto OpenProcessHandle(const HasIdType& hasIdType, DWORD dwDesiredAccess , BOOL bInheritHandle)
		-> ScopeHandle<>
	{
		constexpr static auto isId = std::is_same_v<DWORD, HasIdType>;
		if constexpr (isId) {
			return OpenProcess(dwDesiredAccess, bInheritHandle, hasIdType);
		} else {
			return OpenProcess(dwDesiredAccess, bInheritHandle, hasIdType->th32ProcessID);
		}
	}

	template<typename Type>
	auto SearchProcess(std::basic_string_view<Type> process_name_view)
		-> std::unique_ptr<PROCESSENTRY32<Type>>
	{
		auto process_entry = std::make_unique<PROCESSENTRY32<Type>>();
		process_entry->dwSize = sizeof(PROCESSENTRY32<Type>);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}
		BOOL bFound = Process32First<Type>()(hProcessSnap, process_entry.get());
		while (bFound)
		{
			if (std::basic_string_view<Type>(process_entry->szExeFile) == process_name_view) {
				return process_entry;
			}
			bFound = Process32Next<Type>()(hProcessSnap, process_entry.get());
		}
		return nullptr;
	}

	template<typename Type>
	auto SearchModule(DWORD th32ProcessID, std::basic_string_view<Type> module_name_view)
		-> std::unique_ptr<MODULEENTRY32<Type>>
	{
		auto module_entry = std::make_unique<MODULEENTRY32<Type>>();
		module_entry->dwSize = sizeof(MODULEENTRY32<Type>);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}

		BOOL bFound = Module32First<Type>()(hProcessSnap, module_entry.get());
		while (bFound)
		{
			if (CaseInsensitiveCompare(
				module_name_view, 
				std::basic_string_view<Type>(module_entry->szModule)
			)){
				return module_entry;
			}
			bFound = Module32Next<Type>()(hProcessSnap, module_entry.get());
		};
		return nullptr;
	}


	template<typename T>
	auto ProcessInfo()
		-> std::vector<PROCESSENTRY32<T>>
	{
		std::vector<PROCESSENTRY32<T>> info{};

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return info;
		}

		PROCESSENTRY32<T> process_entry{};
		process_entry.dwSize = sizeof(PROCESSENTRY32<T>);

		BOOL bFound = Process32First<T>()(hProcessSnap, &process_entry);
		while (bFound)
		{
			info.emplace_back(process_entry);
			bFound = Process32Next<T>()(hProcessSnap, &process_entry);
		};
		return info;
	}

	template<typename T>
	auto ModuleInfo(DWORD th32ProcessID) 
		-> std::vector<MODULEENTRY32<T>>
	{
		std::vector<MODULEENTRY32<T>> info{};
		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, th32ProcessID);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return info;
		}

		MODULEENTRY32<T> module_entry{};
		module_entry.dwSize = sizeof(MODULEENTRY32<T>);

		BOOL bFound = Module32First<T>()(hProcessSnap, &module_entry);
		while (bFound){
			info.emplace_back(module_entry);
			bFound = Module32Next<T>()(hProcessSnap, &module_entry);
		};
		return info;
	}
}