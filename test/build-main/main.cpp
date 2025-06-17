#include "Hook.hpp"
#include<HookContext.h>
#include <iostream>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;

class A {

};
class B {

};
class C {

};

void test(A a,A b,A c,B d,C e) {
	a, b, c, d, e;
}
void test1(const char* a, const wchar_t* b, const char8_t* c) {
	a, b, c;
}

int main()
{
	system("chcp 65001");

	auto type_map1 = HookContext(test).refl_args_positions_map<A,B,C>();
	for (auto const& [key, val] : type_map1) {
		std::cout << key << ": ";
		for (auto const& pos : val) {
			std::cout << pos << " ";
		}
		std::cout << std::endl;
	}

	auto type_map2 = HookContext(test1).refl_args_positions_map<const char*, const wchar_t*, const char8_t*>();
	for (auto const& [key, val] : type_map2) {
		if (val.empty()) {
			break;
		}
		std::cout << key << ": ";
		for (auto const& pos : val) {
			std::cout << pos << " ";
		}
		std::cout << std::endl;
	}


	//auto process = SearchProcess(L"Notepad.exe");
	//if (process == nullptr) {
	//	std::cout << "Process not found" << std::endl;
	//	return -1;
	//}
	//std::cout << "Process found: " << process->th32ProcessID << std::endl;
	//auto _module = SearchModule(process->th32ProcessID, L"kernel32.dll");
	//if (_module == nullptr) {
	//	std::cout << "Module not found" << std::endl;
	//	return -1;
	//}
	//std::wcout << L"Module found: " << _module->szModule << std::endl;

	//const auto hProcess = OpenProcessHandle(process);
	//const auto pWriteFile = Exp::GetProcAddressEx(hProcess, _module->hModule, "WriteFile");
	//std::cout << pWriteFile << std::endl;

	//auto pfn = HookPrc(process->th32ProcessID, _module->hModule, "WriteFile", []
	//	{
	//		static int count = 0;
	//		std::cout << "WriteFile called " << count++ << std::endl;
	//	});


	system("pause");
	return 0;
}
