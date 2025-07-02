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

	//using INT_TYPE_LEN = unsigned char; 

	//inline std::map<INT_TYPE, INT_TYPE_LEN> INT_TABLE{
	//	{ INT_TYPE::INT3 , 1 },
	//	{ INT_TYPE::INT1 , 2 }
	//};

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