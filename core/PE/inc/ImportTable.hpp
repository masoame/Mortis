#pragma once
#include "PE.hpp"

namespace Mortis::PE::Imp
{
	auto GetDescriptorGroup(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<IMAGE_IMPORT_DESCRIPTOR>;

	auto GetINTAndIAT(HANDLE ProcessHandle, HMODULE BaseAddress, IMAGE_IMPORT_DESCRIPTOR& ImpDes)
		-> std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>;

	auto GetINTAndIATData(HANDLE ProcessHandle, HMODULE BaseAddress, const std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>& INTAndIAT)
		-> std::vector<std::tuple<Ordinal, Hint, std::string, void*>>;

	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<std::tuple<IMAGE_IMPORT_DESCRIPTOR, std::string, std::vector<std::tuple<Ordinal, Hint, std::string, void*>>>>;

	auto ShowTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> ::std::unique_ptr<std::stringstream>;
}