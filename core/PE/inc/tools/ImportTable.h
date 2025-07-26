#include "PE.h"

namespace Mortis::PE::Imp
{
	struct ImportTable {
		Ordinal _ordinal;
		Hint _hint;
		std::string _import_function_name;
		FuncPtr _address;
	};

	auto GetDescriptorGroup(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<IMAGE_IMPORT_DESCRIPTOR>;

	auto GetINTAndIAT(HANDLE ProcessHandle, HMODULE BaseAddress, IMAGE_IMPORT_DESCRIPTOR& ImpDes)
		-> std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>;

	auto GetINTAndIATData(HANDLE ProcessHandle, HMODULE BaseAddress, const std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>& INTAndIAT)
		-> std::vector<ImportTable>;

	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<
		std::tuple<
		CaseInsensitiveStdString<char>,
		std::vector<ImportTable>
		>>;

	auto GetTableMap(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::map<
		CaseInsensitiveStdString<char>,
		std::vector<ImportTable>
		>;

	auto ShowTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> ::std::unique_ptr<std::stringstream>;
}

#include <ImportTable.hpp>