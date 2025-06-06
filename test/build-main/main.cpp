#include <iostream>
#include "PE.hpp"
#include "Hook.hpp"
#include<string>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::Hook;
int main()
{

	int x = 10;
	{
		ScopeExecutor scope{ [](int& x,int y)->void {
			std::cout << "ScopeExecutor destructor called" << std::endl;
			std::cout << "x = " << x-- << std::endl;
			std::cout << "y = " << y << std::endl;
		},std::ref(x), std::move(x) };
	}
	std::cout << "x = " << x-- << std::endl;

	auto process = SearchProcess(L"notepad.exe");
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
