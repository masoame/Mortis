#pragma once
#include<sstream>
#include <utils.hpp>
#include <asm_types.hpp>

namespace Mortis::PE
{
	using Rva = DWORD;
	using Ordinal = WORD;
	using Hint = WORD;
	using FunctionPtr = LPVOID;

	//搜索进程
	template<typename StringType, typename PROCESSENTRY32Wrapper>
	auto SearchProcess(const StringType& ProcessName)
		-> std::unique_ptr<PROCESSENTRY32Wrapper>;

	//搜索模块
	template<typename StringType, typename MODULEENTRY32Wrapper>
	auto SearchModule(DWORD th32ProcessID, const StringType& ModuleName)
		-> std::unique_ptr<MODULEENTRY32Wrapper>;

	//进程信息
	template<typename T, typename PROCESSENTRY32Wrapper>
	auto ProcessInfo() -> std::vector<PROCESSENTRY32Wrapper>;

	//模块信息
	template<typename T, typename MODULEENTRY32Wrapper>
	auto ModuleInfo(DWORD th32ProcessID) -> std::vector<MODULEENTRY32>;

	//获得DOS头和NT头
	auto GetFileHeader(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::shared_ptr<std::pair<IMAGE_DOS_HEADER, IMAGE_NT_HEADERS>>;

	template<typename T1, typename T2>
	concept SearchProcessConcept = requires (T1 t, T2 p) {
		requires BC::IsString<T1>;
		requires BC::HasType<T2, PROCESSENTRY32, PROCESSENTRY32W>;
	{ p.szExeFile } -> BC::ArrayElementTypeIsSame<T1>;
	};

	template<typename T1, typename T2>
	concept SearchModuleConcept = requires (T1 t, T2 p) {
		requires BC::IsString<T1>;
		requires BC::HasType<T2, MODULEENTRY32, MODULEENTRY32W>;
	{ p.szExePath } -> BC::ArrayElementTypeIsSame<T1>;
	};

	//--data--
	template<bool is_wide>
	struct API : public BaseAPI<is_wide> {
		using Process32FirstWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Process32FirstW>, BT::StaticFunctorWrapper<Process32First>>;
		using Process32NextWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Process32NextW>, BT::StaticFunctorWrapper<Process32Next>>;
		using Module32FirstWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Module32FirstW>, BT::StaticFunctorWrapper<Module32First>>;
		using Module32NextWrapper = std::conditional_t<is_wide, BT::StaticFunctorWrapper<Module32NextW>, BT::StaticFunctorWrapper<Module32Next>>;
	};

	template<bool is_wide>
	struct TYPE : public BaseTYPE<is_wide> {
		using PROCESSENTRY32Wrapper = std::conditional_t<is_wide, PROCESSENTRY32W, PROCESSENTRY32>;
		using MODULEENTRY32Wrapper = std::conditional_t<is_wide, MODULEENTRY32W, MODULEENTRY32>;

	};

	template<typename T1>
	struct SearchProcessWrapper {
		inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
		inline static constexpr bool is_raw_ptr = not BC::HasType<T1, std::wstring, std::string, std::string_view, std::wstring_view>;
		using TYPE = TYPE<is_wide>;
		using API = API<is_wide>;
	};

	template<typename T1>
	struct SearchModuleWrapper {
		inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
		inline static constexpr bool is_raw_ptr = not std::is_same_v<T1, std::conditional_t<is_wide, std::wstring, std::string>>;
		using TYPE = TYPE<is_wide>;
		using API = API<is_wide>;
	};

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	struct ProcessInfoWrapper {
		inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
		using TYPE = TYPE<is_wide>;
		using API = API<is_wide>;
	};

	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	struct ModuleInfoWrapper {
		inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
		using TYPE = TYPE<is_wide>;
		using API = API<is_wide>;
	};


	template<typename ProcessNameType, typename UseWrapper = SearchProcessWrapper<ProcessNameType>, typename PROCESSENTRY32Wrapper = UseWrapper::TYPE::PROCESSENTRY32Wrapper>
		requires SearchProcessConcept<ProcessNameType, PROCESSENTRY32Wrapper>
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

	template<typename ModuleNameType, typename UseWrapper = SearchModuleWrapper<ModuleNameType>, typename MODULEENTRY32Wrapper = UseWrapper::TYPE::MODULEENTRY32Wrapper>
		requires SearchModuleConcept<ModuleNameType, MODULEENTRY32Wrapper>
	auto SearchModule(DWORD th32ProcessID, const ModuleNameType& ModuleName) -> std::unique_ptr<MODULEENTRY32Wrapper>
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


	template<typename T, typename UseWrapper = ProcessInfoWrapper<T>, typename PROCESSENTRY32Wrapper = UseWrapper::TYPE::PROCESSENTRY32Wrapper>
	auto ProcessInfo() -> std::vector<PROCESSENTRY32Wrapper>
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

	template<typename T, typename UseWrapper = ModuleInfoWrapper<T>, typename MODULEENTRY32Wrapper = UseWrapper::MODULEENTRY32Wrapper>
	auto ModuleInfo(DWORD th32ProcessID) -> std::vector<MODULEENTRY32Wrapper>
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