#include "ImportTable.hpp"
#include "asm_types.hpp"

namespace Mortis::PE::Imp
{
	auto GetDescriptorGroup(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<IMAGE_IMPORT_DESCRIPTOR>
	{
		std::vector<IMAGE_IMPORT_DESCRIPTOR> ImpDes{};

		const auto FileHeader = PE::GetFileHeader(ProcessHandle, BaseAddress);
		if (FileHeader == nullptr) {
			return ImpDes;
		}

		ImpDes.emplace_back();
		for (int i = 0;; i++) {
			auto& _ImpDir = ImpDes.back();
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, FileHeader->second.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress + (i * sizeof(IMAGE_IMPORT_DESCRIPTOR))), &_ImpDir, sizeof(IMAGE_IMPORT_DESCRIPTOR), 0) == false) {
				ImpDes.pop_back();
				break;
			}
			if (_ImpDir.Characteristics == 0) {
				ImpDes.pop_back();
				break;
			}
			ImpDes.emplace_back();
		}
		return ImpDes;
	}

	auto GetINTAndIAT(HANDLE ProcessHandle, HMODULE BaseAddress, IMAGE_IMPORT_DESCRIPTOR& ImpDes)
		-> std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>
	{
		std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>> INTAndIAT{};

		for (int i = 0; ; i++) {
			INTAndIAT.emplace_back();
			auto& [_INT, _IAT] = INTAndIAT.back();
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ImpDes.OriginalFirstThunk + (i * sizeof(IMAGE_THUNK_DATA))), &_INT, sizeof(IMAGE_THUNK_DATA), 0) == false) {
				break;
			}
			if (_INT.u1.Ordinal == 0) {
				break;
			}
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ImpDes.FirstThunk + (i * sizeof(IMAGE_THUNK_DATA))), &_IAT, sizeof(IMAGE_THUNK_DATA), 0) == false) {
				break;
			}
		}
		INTAndIAT.pop_back();
		return INTAndIAT;
	}

	auto GetINTAndIATData(HANDLE ProcessHandle, HMODULE BaseAddress, const std::vector<std::pair<IMAGE_THUNK_DATA, IMAGE_THUNK_DATA>>& INTAndIAT)
		-> std::vector<std::tuple<Ordinal, Hint, std::string, void*>>
	{
		char Name[256];
		PIMAGE_IMPORT_BY_NAME IMBN = (PIMAGE_IMPORT_BY_NAME)Name;
		std::vector<std::tuple<Ordinal, Hint, std::string, void*>> Data{};
		for (auto& [INT, IAT] : INTAndIAT) {

			if (IMAGE_SNAP_BY_ORDINAL(INT.u1.Ordinal))
			{
				Data.emplace_back(static_cast<Ordinal>(IMAGE_ORDINAL(INT.u1.Ordinal)), static_cast<Hint>(0), "", reinterpret_cast<void*>(IAT.u1.Function));
			}
			else
			{
				if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, INT.u1.AddressOfData), &Name, sizeof(Name), 0) == false) break;
				Data.emplace_back(static_cast<Ordinal>(0), IMBN->Hint, reinterpret_cast<char*>(IMBN->Name), reinterpret_cast<void*>(IAT.u1.Function));
			}
		}
		return Data;
	}

	auto GetTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::vector<std::tuple<IMAGE_IMPORT_DESCRIPTOR, std::string, std::vector<std::tuple<Ordinal, Hint, std::string, void*>>>>
	{
		char Name[256]{ 0 };
		std::vector<std::tuple<IMAGE_IMPORT_DESCRIPTOR, std::string, std::vector<std::tuple<Ordinal, Hint, std::string, void*>>>> ImportTable{};
		auto ImpDesGroup = GetDescriptorGroup(ProcessHandle, BaseAddress);
		for (auto& ImpDes : ImpDesGroup) {
			if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, ImpDes.Name), &Name, sizeof(Name) - 1, 0) == false)
				return {};

			auto INTAndIAT = GetINTAndIAT(ProcessHandle, BaseAddress, ImpDes);
			auto ImportAddressTable = GetINTAndIATData(ProcessHandle, BaseAddress, INTAndIAT);

			ImportTable.emplace_back(ImpDes, Name, std::move(ImportAddressTable));
		}
		return ImportTable;
	}

	auto ShowImportTable(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::unique_ptr<std::stringstream>
	{
		auto ss = std::make_unique<std::stringstream>();
		auto ImpTable = GetTable(ProcessHandle, BaseAddress);

		for (auto& [ImpDes, Name, ImportAddressTable] : ImpTable) {
			*ss << "\nmoduleName:" << Name << "\n" << std::endl;
			for (auto& [ordinal, hint, name, addr] : ImportAddressTable) {
				*ss << "Ordinal: " << ordinal << " \tHint: " << std::hex << hint << " \tFunctionAddr: " << addr << "  \tFunctionName: " << name << std::endl;
			}
		}
		return ss;
	}
}