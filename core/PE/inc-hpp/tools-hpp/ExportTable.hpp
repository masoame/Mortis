#pragma once
#include "ExportTable.h"
namespace Mortis::PE::Exp
{
	template<typename FuncType>
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncType{
		return reinterpret_cast<FuncType>( GetProcAddressEx(ProcessHandle, BaseAddress, fcName));
	}

	template <typename FuncType>
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncType>{
		return reinterpret_cast<std::vector<FuncType>>(GetProcAddressEx(ProcessHandle, BaseAddress, fcNameGroup));
	}
}