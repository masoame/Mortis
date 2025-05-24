#pragma once
#include "common.hpp"

namespace Mortis
{
	namespace PE
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
			requires IsString<T1>;
			requires HasType<T2, PROCESSENTRY32, PROCESSENTRY32W>;
		{ p.szExeFile } -> ArrayElementTypeIsSame<T1>;
		};

		template<typename T1, typename T2>
		concept SearchModuleConcept = requires (T1 t, T2 p) {
			requires IsString<T1>;
			requires HasType<T2, MODULEENTRY32, MODULEENTRY32W>;
		{ p.szExePath } -> ArrayElementTypeIsSame<T1>;
		};

		//--data--
		template<bool is_wide>
		struct API {
			using Process32FirstWrapper = std::conditional_t<is_wide, Functor<Process32FirstW>, Functor<Process32First>>;
			using Process32NextWrapper = std::conditional_t<is_wide, Functor<Process32NextW>, Functor<Process32Next>>;
			using Module32FirstWrapper = std::conditional_t<is_wide, Functor<Module32FirstW>, Functor<Module32First>>;
			using Module32NextWrapper = std::conditional_t<is_wide, Functor<Module32NextW>, Functor<Module32Next>>;
			using StrCmpWrapper = std::conditional_t<is_wide, Functor<wcscmp>, Functor<strcmp>>;
			using StrCmpIgnoreCaseWrapper = std::conditional_t<is_wide, Functor<wcsicmp>, Functor<stricmp>>;
		};

		template<bool is_wide>
		struct DATA {
			using PROCESSENTRY32Wrapper = std::conditional_t<is_wide, PROCESSENTRY32W, PROCESSENTRY32>;
			using MODULEENTRY32Wrapper = std::conditional_t<is_wide, MODULEENTRY32W, MODULEENTRY32>;
		};

		template<typename T1>
		struct SearchProcessWrapper {
			inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
			inline static constexpr bool is_raw_ptr = not HasType<T1, std::wstring, std::string, std::string_view, std::wstring_view>;
			using DATA = DATA<is_wide>;
			using API = API<is_wide>;
		};

		template<typename T1>
		struct SearchModuleWrapper {
			inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
			inline static constexpr bool is_raw_ptr = not std::is_same_v<T1, std::conditional_t<is_wide, std::wstring, std::string>>;
			using DATA = DATA<is_wide>;
			using API = API<is_wide>;
		};

		template<typename T>
			requires HasType<T, char, wchar_t>
		struct ProcessInfoWrapper {
			inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
			using DATA = DATA<is_wide>;
			using API = API<is_wide>;
		};

		template<typename T>
			requires HasType<T, char, wchar_t>
		struct ModuleInfoWrapper {
			inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
			using DATA = DATA<is_wide>;
			using API = API<is_wide>;
		};


		template<typename T, typename UseWrapper = SearchProcessWrapper<T>, typename PROCESSENTRY32Wrapper = UseWrapper::DATA::PROCESSENTRY32Wrapper>
			requires SearchProcessConcept<T, PROCESSENTRY32Wrapper>
		auto SearchProcess(const T& ProcessName) -> std::unique_ptr<PROCESSENTRY32Wrapper>
		{
			auto pe32 = std::make_unique<PROCESSENTRY32Wrapper>();
			pe32->dwSize = sizeof(*pe32);

			AutoHandle<> hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return nullptr;

			BOOL bRet = UseWrapper::API::Process32FirstWrapper()(hProcessSnap, pe32.get());
			while (bRet)
			{
				if constexpr (UseWrapper::is_raw_ptr) {
					if (!UseWrapper::API::StrCmpWrapper()(pe32->szExeFile, ProcessName))
						return pe32;
				}
				else {
					if (ProcessName == pe32->szExeFile) 
						return pe32;
				}
				bRet = UseWrapper::API::Process32NextWrapper()(hProcessSnap, pe32.get());
			}
			return nullptr;
		}

		template<typename T, typename UseWrapper = SearchModuleWrapper<T>, typename MODULEENTRY32Wrapper = UseWrapper::DATA::MODULEENTRY32Wrapper>
			requires SearchModuleConcept<T, MODULEENTRY32Wrapper>
		auto SearchModule(DWORD th32ProcessID, const T& ModuleName) -> std::unique_ptr<MODULEENTRY32Wrapper>
		{
			auto pe32 = std::make_unique<MODULEENTRY32Wrapper>();
			pe32->dwSize = sizeof(*pe32);

			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return nullptr;

			BOOL bMore = UseWrapper::API::Module32FirstWrapper()(hProcessSnap, pe32.get());
			while (bMore)
			{
				if constexpr (UseWrapper::is_raw_ptr) {
					if (!UseWrapper::API::StrCmpIgnoreCaseWrapper()(pe32->szModule, ModuleName)) 
						return pe32;
				}
				else {
					if (!UseWrapper::API::StrCmpIgnoreCaseWrapper()(pe32->szModule, ModuleName.c_str()))
						return pe32;
				}
				bMore = UseWrapper::API::Module32NextWrapper()(hProcessSnap, pe32.get());
			};
			return nullptr;
		}

		template<typename T, typename UseWrapper = ProcessInfoWrapper<T>, typename PROCESSENTRY32Wrapper = UseWrapper::DATA::PROCESSENTRY32Wrapper>
		auto ProcessInfo() -> std::vector<PROCESSENTRY32Wrapper>
		{
			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return {};

			PROCESSENTRY32Wrapper pe32{};
			pe32.dwSize = sizeof(pe32);

			std::vector<PROCESSENTRY32Wrapper> ret{};

			BOOL bMore = UseWrapper::API::Process32FirstWrapper()(hProcessSnap, &pe32);
			while (bMore)
			{
				ret.emplace_back(pe32);
				bMore = UseWrapper::API::Process32NextWrapper()(hProcessSnap, &pe32);
			};
			return ret;
		}

		template<typename T, typename UseWrapper = ModuleInfoWrapper<T>, typename MODULEENTRY32Wrapper = UseWrapper::MODULEENTRY32Wrapper>
		auto ModuleInfo(DWORD th32ProcessID) -> std::vector<MODULEENTRY32Wrapper>
		{
			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, th32ProcessID);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return {};

			std::vector<MODULEENTRY32Wrapper> ret{};

			MODULEENTRY32Wrapper pe32{};
			pe32.dwSize = sizeof(pe32);

			BOOL bMore = UseWrapper::API::Module32FirstWrapper()(hProcessSnap, &pe32);
			while (bMore)
			{
				ret.emplace_back(pe32);
				bMore = UseWrapper::API::Module32NextWrapper()(hProcessSnap, &pe32);
			};
			return ret;
		}
	}
}