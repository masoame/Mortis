#pragma once
#include "common.hpp"
#include<windows.h>
#include<tlhelp32.h>

#include<string>
#include<vector>
#include<memory>

#include<functional>
#include<sstream>

namespace Mortis
{
	namespace PE
	{
		using RVA = DWORD;
		using Ordinal = WORD;
		using Hint = WORD;
		using FunctionPtr = LPVOID;

		//搜索进程
		template<typename StringType, typename _PROCESSENTRY32>
		auto SearchProcess(const StringType& ProcessName)
			-> std::unique_ptr<_PROCESSENTRY32>;

		//搜索模块
		template<typename StringType, typename _MODULEENTRY32>
		auto SearchModule(DWORD th32ProcessID, const StringType& ModuleName)
			-> std::unique_ptr<_MODULEENTRY32>;

		//进程信息
		template<typename T, typename _PROCESSENTRY32>
		auto ProcessInfo() -> std::vector<_PROCESSENTRY32>;

		//模块信息
		template<typename T, typename _MODULEENTRY32>
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

		template<bool is_wide>
		using _Process32First = std::conditional_t<is_wide, Functor<Process32FirstW>, Functor<Process32First>>;
		template<bool is_wide>
		using _Process32Next = std::conditional_t<is_wide, Functor<Process32NextW>, Functor<Process32Next>>;
		template<bool is_wide>
		using _Module32First = std::conditional_t<is_wide, Functor<Module32FirstW>, Functor<Module32First>>;
		template<bool is_wide>
		using _Module32Next = std::conditional_t<is_wide, Functor<Module32NextW>, Functor<Module32Next>>;
		template<bool is_wide>
		using _CMPFUNC_RAW = std::conditional_t<is_wide, Functor<wcscmp>, Functor<strcmp>>;
		template<bool is_wide>
		using _CMPIFUNC_RAW = std::conditional_t<is_wide, Functor<wcsicmp>, Functor<stricmp>>;
		//--data--
		template<bool is_wide>
		struct API {
			using _Process32First = _Process32First<is_wide>;
			using _Process32Next = _Process32Next<is_wide>;
			using _Module32First = _Module32First<is_wide>;
			using _Module32Next = _Module32Next<is_wide>;
			using _CMPFUNC_RAW = _CMPFUNC_RAW<is_wide>;
			using _CMPIFUNC_RAW = _CMPIFUNC_RAW<is_wide>;
		};

		template<typename T1>
		struct SearchProcessData {
			inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
			inline static constexpr bool is_raw_ptr = not HasType<T1, std::wstring, std::string, std::string_view, std::wstring_view>;
			using _PROCESSENTRY32 = std::conditional_t<is_wide, PROCESSENTRY32W, PROCESSENTRY32>;
			using API = API<is_wide>;
		};

		template<typename T1>
		struct SearchModuleData {
			inline static constexpr bool is_wide = not (std::is_convertible_v<T1, std::string> || std::is_same_v<T1, std::string_view>);
			inline static constexpr bool is_raw_ptr = not std::is_same_v<T1, std::conditional_t<is_wide, std::wstring, std::string>>;
			using _MODULEENTRY32 = std::conditional_t<is_wide, MODULEENTRY32W, MODULEENTRY32>;
			using API = API<is_wide>;
		};

		template<typename T>
			requires HasType<T, char, wchar_t>
		struct ProcessInfoData {
			inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
			using _PROCESSENTRY32 = std::conditional_t<is_wide, PROCESSENTRY32W, PROCESSENTRY32>;
			using API = API<is_wide>;
		};

		template<typename T>
			requires HasType<T, char, wchar_t>
		struct ModuleInfoData {
			inline static constexpr bool is_wide = std::is_same_v<T, wchar_t>;
			using _MODULEENTRY32 = std::conditional_t<is_wide, MODULEENTRY32W, MODULEENTRY32>;
			using API = API<is_wide>;
		};


		template<typename T, typename _Static_Data = SearchProcessData<T>, typename _PROCESSENTRY32 = _Static_Data::_PROCESSENTRY32>
			requires SearchProcessConcept<T, _PROCESSENTRY32>
		auto SearchProcess(const T& ProcessName) -> std::unique_ptr<_PROCESSENTRY32>
		{
			auto pe32 = std::make_unique<_PROCESSENTRY32>();
			pe32->dwSize = sizeof(*pe32);

			AutoHandle<> hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return nullptr;

			BOOL bRet = _Static_Data::API::_Process32First()(hProcessSnap, pe32.get());
			while (bRet)
			{
				if constexpr (_Static_Data::is_raw_ptr) {
					if (!_Static_Data::API::_CMPFUNC_RAW()(pe32->szExeFile, ProcessName))
						return pe32;
				}
				else {
					if (ProcessName == pe32->szExeFile) 
						return pe32;
				}
				bRet = _Static_Data::API::_Process32Next()(hProcessSnap, pe32.get());
			}
			return nullptr;
		}

		template<typename T, typename _Static_Data = SearchModuleData<T>, typename _MODULEENTRY32 = _Static_Data::_MODULEENTRY32>
			requires SearchModuleConcept<T, _MODULEENTRY32>
		auto SearchModule(DWORD th32ProcessID, const T& ModuleName) -> std::unique_ptr<_MODULEENTRY32>
		{
			auto pe32 = std::make_unique<_MODULEENTRY32>();
			pe32->dwSize = sizeof(*pe32);

			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return nullptr;

			BOOL bMore = _Static_Data::API::_Module32First()(hProcessSnap, pe32.get());
			while (bMore)
			{
				if constexpr (_Static_Data::is_raw_ptr) {
					if (!_Static_Data::API::_CMPIFUNC_RAW()(pe32->szModule, ModuleName)) 
						return pe32;
				}
				else {
					if (!_Static_Data::API::_CMPIFUNC_RAW()(pe32->szModule, ModuleName.c_str()))
						return pe32;
				}
				bMore = _Static_Data::API::_Module32Next()(hProcessSnap, pe32.get());
			};
			return nullptr;
		}

		template<typename T, typename _Static_Data = ProcessInfoData<T>, typename _PROCESSENTRY32 = _Static_Data::_PROCESSENTRY32>
		auto ProcessInfo() -> std::vector<_PROCESSENTRY32>
		{
			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return {};

			_PROCESSENTRY32 pe32{};
			pe32.dwSize = sizeof(pe32);

			std::vector<_PROCESSENTRY32> ret{};

			BOOL bMore = _Static_Data::API::_Process32First()(hProcessSnap, &pe32);
			while (bMore)
			{
				ret.emplace_back(pe32);
				bMore = _Static_Data::API::_Process32Next()(hProcessSnap, &pe32);
			};
			return ret;
		}

		template<typename T, typename _Static_Data = ModuleInfoData<T>, typename _MODULEENTRY32 = _Static_Data::_MODULEENTRY32>
		auto ModuleInfo(DWORD th32ProcessID) -> std::vector<_MODULEENTRY32>
		{
			AutoHandle hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, th32ProcessID);
			if (hProcessSnap == INVALID_HANDLE_VALUE) return {};

			std::vector<_MODULEENTRY32> ret{};

			_MODULEENTRY32 pe32{};
			pe32.dwSize = sizeof(pe32);

			BOOL bMore = _Static_Data::API::_Module32First()(hProcessSnap, &pe32);
			while (bMore)
			{
				ret.emplace_back(pe32);
				bMore = _Static_Data::API::_Module32Next()(hProcessSnap, &pe32);
			};
			return ret;
		}
	}
}