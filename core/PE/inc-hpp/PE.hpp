#pragma once
#include<PE.h>
namespace Mortis::PE
{
	template<typename HasIdType>
	auto OpenProcessHandle(const HasIdType& hasIdType, DWORD dwDesiredAccess , BOOL bInheritHandle)
		-> ScopeHandle<>
	{
		constexpr const static auto isId = std::is_same_v<DWORD, HasIdType>;
		if constexpr (isId) {
			return OpenProcess(dwDesiredAccess, bInheritHandle, hasIdType);
		} else {
			return OpenProcess(dwDesiredAccess, bInheritHandle, hasIdType->th32ProcessID);
		}
	}

	template<typename T>
	auto SearchProcess(std::basic_string_view<T> process_name_view)
		-> std::unique_ptr<PROCESSENTRY32<T>>
	{
		auto process_entry = std::make_unique<PROCESSENTRY32<T>>();
		process_entry->dwSize = sizeof(PROCESSENTRY32<T>);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}
		BOOL bFound = Process32First<T>()(hProcessSnap, process_entry.get());
		while (bFound)
		{
			if (std::basic_string_view<T>(process_entry->szExeFile) == process_name_view) {
				return process_entry;
			}
			bFound = Process32Next<T>()(hProcessSnap, process_entry.get());
		}
		return nullptr;
	}

	template<typename T>
	auto SearchModule(DWORD th32ProcessID, std::basic_string_view<T> module_name_view)
		-> std::unique_ptr<MODULEENTRY32<T>>
	{
		auto module_entry = std::make_unique<MODULEENTRY32<T>>();
		module_entry->dwSize = sizeof(MODULEENTRY32<T>);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}

		BOOL bFound = Module32First<T>()(hProcessSnap, module_entry.get());
		while (bFound)
		{
			if (CaseInsensitiveCompare(
				module_name_view, 
				std::basic_string_view<T>(module_entry->szModule)
			)){
				return module_entry;
			}
			bFound = Module32Next<T>()(hProcessSnap, module_entry.get());
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
	auto ProcessInfoMap(EnumInfoMapType key_type)
		-> std::map<
		std::variant<ProcessId, std::basic_string<T>>, 
		SingleOrMultiple<PROCESSENTRY32<T>>
		>
	{
		std::map<
			std::variant<ProcessId, std::basic_string<T>>, 
			SingleOrMultiple<PROCESSENTRY32<T>>
		> info_map;

		const auto info_arr = ProcessInfo<T>();

		switch (key_type)
		{
		case PROCESS_ID:

			for (const PROCESSENTRY32<T>& info : info_arr){
				if (info_map.contains(info.th32ProcessID) == true) {
					throw std::exception("error repeat th32ProcessID");
				}
				info_map.emplace(info.th32ProcessID, info);
			}

			break;
		case PROCESS_SZExeFile:

			for (const PROCESSENTRY32<T>& info : info_arr){
				std::basic_string<T> szExeFile(info.szExeFile);
				if (const auto iter = info_map.find(szExeFile); iter != info_map.cend()) {
					iter->second.emplace_back(info);
				}
				info_map.emplace(std::move(szExeFile), info);
			}

			break;

		default:
			throw std::exception();
		}
		return info_map;
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

	template<typename T>
	auto ModuleInfoMap(DWORD th32ProcessID, EnumInfoMapType key_type)
		-> std::map<std::variant<HMODULE, CaseInsensitiveStdString<T>>, MODULEENTRY32<T>>
	{
		std::map<std::variant<HMODULE, CaseInsensitiveStdString<T>>, MODULEENTRY32<T>> info_map;
		auto info_arr = ModuleInfo<T>(th32ProcessID);
		switch (key_type)
		{
		case MODULE_HMODULE:
			for (const MODULEENTRY32<T>& info : info_arr)
			{
				if (info_map.contains(info.hModule) == true) {
					throw std::exception("error repeat hModule");
				}
				info_map.emplace(info.hModule, info);
			}
			break;
		case MODULE_SZModule:
			for (const MODULEENTRY32<T>& info : info_arr)
			{
				std::basic_string_view<T> szModule = info.szModule;
				if (auto iter = info_map.find(szModule); iter != info_map.cend()) {
					iter->second = info;
				}
				info_map.emplace(szModule, info);
			}
			break;
		default:
			throw std::exception();
		}
		return info_map;
	}
}