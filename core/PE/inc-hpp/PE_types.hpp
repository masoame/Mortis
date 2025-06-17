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

	enum class IntCode {
		INT3 = 0xCC
	};

	//constexpr const BYTE INT3 = 0xCC;
	constexpr const BYTE NOP = 0x90;
	constexpr const BYTE RET = 0xC3;

	using Rva = DWORD;
	using Ordinal = WORD;
	using Hint = WORD;
	using FuncPtr = LPVOID;
	using FuncArgPos = unsigned char;


	template<bool is_wide>
	struct TYPE : public BaseTYPE<is_wide> {
		using PROCESSENTRY32Wrapper = std::conditional_t<is_wide, PROCESSENTRY32W, PROCESSENTRY32>;
		using MODULEENTRY32Wrapper = std::conditional_t<is_wide, MODULEENTRY32W, MODULEENTRY32>;

	};
}