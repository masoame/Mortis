#include "Hook.hpp"

#include <iostream>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;

int main()
{
	system("chcp 65001");

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

	const auto hProcess = OpenProcessHandle(process);
	const auto pWriteFile = Exp::GetProcAddressEx(hProcess, _module->hModule, "WriteFile");
	std::cout << pWriteFile << std::endl;

	auto pfn = HookPrc(process->th32ProcessID, _module->hModule, "WriteFile",[]
	{
		static int count = 0;
		std::cout << "WriteFile called " << count++ << std::endl;
	});


	system("pause");
	return pfn;
}
