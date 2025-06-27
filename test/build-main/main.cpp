#include "Hook.hpp"
#include<HookContext.h>
#include<DbgExecuter.hpp>
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


void test1(const char* a, const wchar_t* b, const char8_t* c) {
	a, b, c;
}

int main()
{
	system("chcp 65001");

	auto process_entry = SearchProcess(L"Notepad.exe");
	if (process_entry == nullptr) {
		std::cout << "Process not found" << std::endl;
		return -1;
	}
	std::cout << "Process found: " << process_entry->th32ProcessID << std::endl;
	auto _module = SearchModule(process_entry->th32ProcessID, L"kernel32.dll");
	if (_module == nullptr) {
		std::cout << "Module not found" << std::endl;
		return -1;
	}
	std::wcout << L"Module found: " << _module->szModule << std::endl;

	const auto hProcess = OpenProcessHandle(process_entry);
	const auto pWriteFile = Exp::GetProcAddressEx(hProcess, _module->hModule, "WriteFile");
	std::cout << pWriteFile << std::endl;

	auto dbgContext = std::make_unique<DebugContext>();
	dbgContext->_fp_exception_address = pWriteFile;
	dbgContext->_dw_exception_code = EXCEPTION_BREAKPOINT;

	auto dbgExecutor = std::make_shared<DbgExecuter>(process_entry->th32ProcessID);
	dbgExecutor->regDbgContext(std::move(dbgContext));

	//auto pfn = HookPrc(process->th32ProcessID, _module->hModule, "WriteFile", []{
	//	static int count = 0;
	//	std::cout << "WriteFile called " << count++ << std::endl;
	//});


	system("pause");
	return true;
}
