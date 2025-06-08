#pragma once
#include<PE.h>

namespace Mortis::PE
{
	template<typename HasIdType>
		requires BC::HasType<HasIdType, PROCESSENTRY32*, PROCESSENTRY32W*, std::unique_ptr<PROCESSENTRY32>, std::unique_ptr<PROCESSENTRY32W>, DWORD>
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

	template<typename ProcessNameType, typename UseWrapper , typename PROCESSENTRY32Wrapper>
		requires BC::SearchProcessConcept<ProcessNameType, PROCESSENTRY32Wrapper>
	auto SearchProcess(const ProcessNameType& processName) -> std::unique_ptr<PROCESSENTRY32Wrapper>
	{
		typename UseWrapper::TYPE::StringViewWrapper process_name_view{ processName };

		auto process_entry = std::make_unique<PROCESSENTRY32Wrapper>();
		process_entry->dwSize = sizeof(PROCESSENTRY32Wrapper);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}
		BOOL bFound = UseWrapper::API::Process32FirstWrapper()(hProcessSnap, process_entry.get());
		while (bFound)
		{
			if (UseWrapper::TYPE::StringViewWrapper(process_entry->szExeFile) == process_name_view) {
				return process_entry;
			}
			bFound = UseWrapper::API::Process32NextWrapper()(hProcessSnap, process_entry.get());
		}
		return nullptr;
	}

	template<typename ModuleNameType, typename UseWrapper, typename MODULEENTRY32Wrapper>
		requires BC::SearchModuleConcept<ModuleNameType, MODULEENTRY32Wrapper>
	auto SearchModule(DWORD th32ProcessID, const ModuleNameType& ModuleName) 
		-> std::unique_ptr<MODULEENTRY32Wrapper>
	{
		typename UseWrapper::TYPE::StringViewWrapper module_name_view{ ModuleName };
		auto module_entry = std::make_unique<MODULEENTRY32Wrapper>();
		module_entry->dwSize = sizeof(MODULEENTRY32Wrapper);

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return nullptr;
		}

		BOOL bFound = UseWrapper::API::Module32FirstWrapper()(hProcessSnap, module_entry.get());
		while (bFound)
		{
			if (UseWrapper::TYPE::StringViewWrapper(module_entry->szModule) == module_name_view) {
				return module_entry;
			}
			bFound = UseWrapper::API::Module32NextWrapper()(hProcessSnap, module_entry.get());
		};
		return nullptr;
	}


	template<typename T, typename UseWrapper, typename PROCESSENTRY32Wrapper>
	auto ProcessInfo()
		-> std::vector<PROCESSENTRY32Wrapper>
	{
		std::vector<PROCESSENTRY32Wrapper> info{};

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return info;
		}

		PROCESSENTRY32Wrapper process_entry{};
		process_entry.dwSize = sizeof(PROCESSENTRY32Wrapper);

		BOOL bFound = UseWrapper::API::Process32FirstWrapper()(hProcessSnap, &process_entry);
		while (bFound)
		{
			info.emplace_back(process_entry);
			bFound = UseWrapper::API::Process32NextWrapper()(hProcessSnap, &process_entry);
		};
		return info;
	}

	template<typename T, typename UseWrapper, typename MODULEENTRY32Wrapper>
	auto ModuleInfo(DWORD th32ProcessID) 
		-> std::vector<MODULEENTRY32Wrapper>
	{
		std::vector<MODULEENTRY32Wrapper> info{};

		ScopeHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, th32ProcessID);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return info;
		}

		MODULEENTRY32Wrapper module_entry{};
		module_entry.dwSize = sizeof(MODULEENTRY32Wrapper);

		BOOL bFound = UseWrapper::API::Module32FirstWrapper()(hProcessSnap, &module_entry);
		while (bFound){
			info.emplace_back(module_entry);
			bFound = UseWrapper::API::Module32NextWrapper()(hProcessSnap, &module_entry);
		};
		return info;
	}
}