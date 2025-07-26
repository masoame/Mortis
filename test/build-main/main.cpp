#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;

int main() {

	auto info_map = ProcessInfoMap(PROCESS_SZExeFile);

	if (info_map["Notepad.exe"].is_single()) {
		auto module_map = ModuleInfoMap(info_map["Notepad.exe"].get_single().th32ProcessID, MODULE_SZModule);
		::spdlog::info("find module name: {}", module_map["KerNel32.DLL"].szModule);
		::spdlog::info("find module name: {}", (void*)module_map["kernel32.dll"].hModule);
	}

	return 0;
}
