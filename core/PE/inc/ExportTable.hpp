#pragma once
#include "PE.hpp"

#include <vector>

namespace Mortis::PE::Exp
{
	//获得导出表目录
	auto GetDirectory(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::unique_ptr<IMAGE_EXPORT_DIRECTORY>;

	//获得导出函数名称
	auto GetNameOfRVAGroup(HANDLE ProcessHandle, HMODULE BaseAddress, const std::unique_ptr<IMAGE_EXPORT_DIRECTORY>& ExpDir = nullptr)
		-> std::vector<Rva>;

	//获得导入表
	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<std::tuple<Ordinal, Rva, std::string>>;

	//显示导出表
	auto ShowTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::stringstream;

	//查找导出函数地址
	template<typename FuncPtr>
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncPtr;

	//批量找出导出函数地址
	template <typename FuncPtr>
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncPtr>;

	template<typename FuncPtr = PE::FunctionPtr>
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

	template <typename FuncPtr = PE::FunctionPtr>
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