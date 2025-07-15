#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;


int main() {

	auto process_info_map = ProcessInfoMap(PROCESS_SZExeFile);

	bool isSingle;
	if (process_info_map.contains("notepad.exe")) {
		isSingle = process_info_map["notepad.exe"].isSingle();
		isSingle ?
			std::cout << "notepad.exe is single" << std::endl :
			std::cout << "notepad.exe is multiple" << std::endl;
	}else {
		std::cout << "not found" << std::endl;
		return -1;
	}

	if (isSingle) {
		auto module_info_map = ModuleInfoMap(process_info_map["notepad.exe"].getValue().th32ProcessID, MODULE_NAME_LOWER);
		std::cout << "image base address : " << module_info_map["kernel32.dll"].hModule << std::endl;
		std::cout << "image base name : " << module_info_map["kernel32.dll"].szModule << std::endl;
	}

	return 0;
}
