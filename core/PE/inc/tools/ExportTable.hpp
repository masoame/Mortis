#pragma once
#include "ExportTable.h"
namespace Mortis::PE::Exp
{
	template<typename FuncPtr = FunctionPtr>
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncPtr
	{
		auto ExpTable = GetTable(ProcessHandle, BaseAddress);
		for (auto& [ordinal, addr, name] : ExpTable) {
			if (name == fcName) {
				return reinterpret_cast<FuncPtr>(MakeAddress(BaseAddress, addr));
			}
		}
		return FuncPtr{};
	}

	template <typename FuncPtr = FunctionPtr>
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncPtr>
	{
		auto ExpTable = GetTable(ProcessHandle, BaseAddress);
		std::vector<FuncPtr> result;
		for (const auto& [ordinal, addr, name] : ExpTable) {
			if (std::find(fcNameGroup.begin(), fcNameGroup.end(), name) != fcNameGroup.end()) {
				result.emplace_back(reinterpret_cast<FuncPtr>(MakeAddress(BaseAddress, addr)));
			}
		}
		return result;
	}
}