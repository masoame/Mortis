#include "ExportTable.hpp"
#include "asm_types.hpp"

#include<sstream>

namespace Mortis::PE::Exp
{
	auto GetDirectory(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::unique_ptr<IMAGE_EXPORT_DIRECTORY>
	{
		auto FileHeader = GetFileHeader(ProcessHandle, BaseAddress);
		if (FileHeader == nullptr) {
			return nullptr;
		}
		auto ExpDir = std::make_unique<IMAGE_EXPORT_DIRECTORY>();
		if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, FileHeader->second.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress), ExpDir.get(), sizeof(IMAGE_EXPORT_DIRECTORY), 0) == false) {
			return nullptr;
		}
		return ExpDir;
	}

	auto GetNameOfRVAGroup(HANDLE ProcessHandle, HMODULE BaseAddress, const std::unique_ptr<IMAGE_EXPORT_DIRECTORY>& ExpDir)
		-> std::vector<Rva>
	{
		if (ExpDir == nullptr) {
			return {};
		}
		std::vector<Rva> Namestable;
		Namestable.resize(ExpDir->NumberOfNames);

		if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ExpDir->AddressOfNames), &Namestable[0], ExpDir->NumberOfNames * sizeof(int32_t), 0) == false) {
			return {};
		}
		return Namestable;
	}

	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<ExportTable>
	{
		auto ExpDir = GetDirectory(ProcessHandle, BaseAddress);
		if (ExpDir == nullptr) {
			return {};
		}
		auto Namestable = GetNameOfRVAGroup(ProcessHandle, BaseAddress, ExpDir);
		if (Namestable.empty()) {
			return {};
		}
		std::vector<ExportTable> ExportTable{ ExpDir->NumberOfNames,{} };

		std::array<char, 256> ProcName;

		for (DWORD i = 0; i != ExpDir->NumberOfNames; i++) {

			auto& [ordinal, rva, procName] = ExportTable[i];

			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, Namestable[i]), ProcName.data(), ProcName.size() - 1, 0) == false) {
				return {};
			}

			procName = ProcName.data();

			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ExpDir->AddressOfNameOrdinals + sizeof(int16_t) * i), &ordinal, sizeof(int16_t), 0) == false) {
				return {};
			}
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ExpDir->AddressOfFunctions + (ordinal - ExpDir->Base + 1) * sizeof(int32_t)), &rva, sizeof(int32_t), 0) == false) {
				return {};
			}
		}
		return ExportTable;
	}

	auto ShowTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::stringstream
	{
		std::stringstream ss;
		for (const auto& [ordinal, addr, name] : GetTable(ProcessHandle, BaseAddress)) {
			ss << std::hex << ordinal << " \t" << addr << " \t" << name << std::endl;
		}
		return ss;
	}

	auto GetProcAddressEx(HANDLE ProcessHandle, HMODULE BaseAddress, std::string_view fcName)
		-> FuncPtr
	{
		const auto ExpTable = GetTable(ProcessHandle, BaseAddress);
		for (const auto& [ordinal, addr, name] : ExpTable) {
			if (name == fcName) {
				return MakeAddress<FuncPtr>(BaseAddress, addr);
			}
		}
		return nullptr;
	}

	auto GetProcAddressEx(const HANDLE ProcessHandle, const HMODULE BaseAddress, const std::vector<std::string_view>& fcNameGroup)
		-> std::vector<FuncPtr>
	{
		const auto ExpTable = GetTable(ProcessHandle, BaseAddress);
		std::vector<FuncPtr> result;
		for (const auto& [ordinal, addr, name] : ExpTable) {
			if (std::find(fcNameGroup.begin(), fcNameGroup.end(), name) != fcNameGroup.end()) {
				result.emplace_back(MakeAddress<FuncPtr>(BaseAddress, addr));
			}
		}
		return result;
	}
}