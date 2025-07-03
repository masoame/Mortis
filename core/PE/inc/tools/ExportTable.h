#pragma once
#include "PE.h"

namespace Mortis::PE::Exp
{
	struct ExportTable
	{
		Ordinal _ordinal;
		Rva _rva;
		std::string _func_name;
	};

	//获得导出表目录
	auto GetDirectory(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::unique_ptr<IMAGE_EXPORT_DIRECTORY>;

	//获得导出函数名称
	auto GetNameOfRVAGroup(HANDLE ProcessHandle, HMODULE BaseAddress, const std::unique_ptr<IMAGE_EXPORT_DIRECTORY>& ExpDir = nullptr)
		-> std::vector<Rva>;

	//获得导出表
	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<std::tuple<Ordinal, Rva, std::string>>;

	//显示导出表
	auto ShowTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::stringstream;

	//导出函数地址
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncPtr;
	template<typename FuncType>
	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncType;

	//批量找出导出函数地址
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncPtr>;
	template <typename FuncType>
	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncType>;
}

#include<ExportTable.hpp>