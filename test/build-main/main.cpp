#include <iostream>
#include "PE.hpp"
#include "Hook.hpp"
#include<string>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::Hook;

struct A {
	int _a;
	int _b;
	A() {};
	~A() {
		std::cout << _a << std::endl;
		std::cout << "delete A" << std::endl;
	}
};

void DeleterA(A* a) {
	delete a;

}

using FunctorA = BT::StaticFunctorWrapper<DeleterA>;

int main()
{
	system("chcp 65001");
	{
		ScopeHandle<A, FunctorA> a = new A{};
	}

	auto process = SearchProcess("Notepad.exe");
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
