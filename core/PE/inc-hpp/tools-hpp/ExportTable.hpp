#pragma once
#include "ExportTable.h"
namespace Mortis::PE::Exp
{
	template<typename FuncPtr>
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncPtr{
		return reinterpret_cast<FuncPtr>( GetProcAddressEx(ProcessHandle, BaseAddress, fcName));
	}

	template <typename FuncPtr>
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncPtr>{
		return reinterpret_cast<std::vector<FuncPtr>>(GetProcAddressEx(ProcessHandle, BaseAddress, fcNameGroup));
	}
}