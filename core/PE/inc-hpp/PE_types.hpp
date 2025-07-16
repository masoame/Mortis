#pragma once
#include "base_types.hpp"
#include <Windows.h>

namespace Mortis::PE
{
#ifndef _WIN64
	using HOOKWORD = DWORD32;
#else
	using HOOKWORD = DWORD64;
#endif

	enum class INT_TYPE {
		INT3 = 0xCC,
		INT1 = 0xCD01,
	};

	using Rva = DWORD;
	using Ordinal = WORD;
	using Hint = WORD;
	using FuncPtr = LPVOID;
	using FuncArgPos = unsigned char;

	using ProcessId = DWORD;

	enum EnumInfoMapType
	{
		PROCESS_ID,
		PROCESS_SZExeFile,

		MODULE_HMODULE,
		MODULE_SZModule,
	};


	template<typename T = char>
		requires BC::HasType<T, char, wchar_t>
	using PROCESSENTRY32 = std::conditional_t<std::is_same_v<T,char>, ::PROCESSENTRY32, ::PROCESSENTRY32W>;


	template<typename T>
		requires BC::HasType<T, char, wchar_t>
	using MODULEENTRY32 = std::conditional_t<std::is_same_v<T, char>, ::MODULEENTRY32, ::MODULEENTRY32W>;
}

