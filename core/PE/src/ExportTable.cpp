#include "ExportTable.hpp"
#include "asm_types.hpp"

#include<array>

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
		-> std::vector<std::tuple<Ordinal, Rva, std::string>>
	{
		auto ExpDir = GetDirectory(ProcessHandle, BaseAddress);
		if (ExpDir == nullptr) {
			return {};
		}
		auto Namestable = GetNameOfRVAGroup(ProcessHandle, BaseAddress, ExpDir);
		if (Namestable.empty()) {
			return {};
		}
		std::vector<std::tuple<Ordinal, Rva, std::string>> ExportTable{ ExpDir->NumberOfNames ,{} };

		std::array<char, 256> ProcName;
		Rva Prc_RVA = 0;
		Ordinal ordinals = 0;

		for (DWORD i = 0; i != ExpDir->NumberOfNames; i++) {

			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, Namestable[i]), ProcName.data(), ProcName.size() - 1, 0) == false) {
				return {};
			}
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ExpDir->AddressOfNameOrdinals + sizeof(int16_t) * i), &ordinals, sizeof(int16_t), 0) == false) {
				return {};
			}
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ExpDir->AddressOfFunctions + (ordinals - ExpDir->Base + 1) * sizeof(int32_t)), &Prc_RVA, sizeof(int32_t), 0) == false) {
				return {};
			}

			std::get<0>(ExportTable[i]) = static_cast<Ordinal>(ordinals + ExpDir->Base);
			std::get<1>(ExportTable[i]) = Prc_RVA;
			std::get<2>(ExportTable[i]) = ProcName.data();
		}
		return ExportTable;
	}

	auto ShowExportTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::stringstream
	{
		std::stringstream ss;
		for (const auto& [ordinal, addr, name] : GetTable(ProcessHandle, BaseAddress)) {
			ss << std::hex << ordinal << " \t" << addr << " \t" << name << std::endl;
		}
		return ss;
	}
}