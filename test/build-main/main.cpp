#include <iostream>
#include "PE.hpp"
#include "Hook.hpp"
#include<string>

using namespace Mortis::PE;
using namespace Mortis::Hook;
int main()
{
	auto process = SearchProcess(L"Notepad.exe");
	if (process == nullptr) {
		std::cout << "Process not found" << std::endl;
		return -1;
	}
	std::cout << "Process found: " << process->th32ProcessID << std::endl;
	auto _module = SearchModule(process->th32ProcessID, L"kernel32.dll");
	if (_module == nullptr) {
		std::cout << "Module not found" << std::endl;
		return -1;
	}
	std::wcout << L"Module found: " << _module->szModule << std::endl;
	auto pfn = HookPrc(process->th32ProcessID, _module->hModule, "WriteFile",[]
	{
		static int count = 0;
		std::cout << "WriteFile called " << count++ << std::endl;
	});


	system("pause");
	return pfn;
}
