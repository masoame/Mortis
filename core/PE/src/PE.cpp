#include "PE.hpp"


namespace Mortis::PE {

	auto GetFileHeader(HANDLE ProcessHandle, HMODULE BaseAddress)
		-> std::shared_ptr<std::pair<IMAGE_DOS_HEADER, IMAGE_NT_HEADERS>>
	{
		if (!ProcessHandle || !BaseAddress)
			return nullptr;

		auto FileHeader = std::make_shared<std::pair<IMAGE_DOS_HEADER, IMAGE_NT_HEADERS>>();

		if ((ReadProcessMemory(ProcessHandle, BaseAddress, &FileHeader->first, sizeof(IMAGE_DOS_HEADER), 0) == false)
			|| (FileHeader->first.e_magic != 0x5A4D)) {
			return nullptr;
		}
		if (ReadProcessMemory(ProcessHandle, MakeAddress(BaseAddress, FileHeader->first.e_lfanew), &FileHeader->second, sizeof(IMAGE_NT_HEADERS), 0) == false
			|| (FileHeader->second.Signature != 0x4550)) {
			return nullptr;
		}
		return FileHeader;
	}
}