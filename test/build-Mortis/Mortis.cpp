#include "Hook.hpp"
#include<DbgExecuter.hpp>
#include <iostream>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


using namespace std::chrono_literals;
int main()
{
	system("chcp 65001");

	std::wstring_view process_name(L"Notepad.exe");
	auto process_entry = SearchProcess(process_name);
	if (process_entry == nullptr) {
		std::cout << "Process not found" << std::endl;
		return -1;
	}

	auto dbgContext = std::make_unique<DebugContext>(&WriteFile);

	dbgContext->regExceptionCallBack(
		[](const DebugContext & ctx) {
			ctx;
			static int count = 0;
			spdlog::info("count: {}", ++count);
		}
	);
	auto dbgExecutor = std::make_shared<DbgExecuter>(process_entry->th32ProcessID);
	dbgExecutor->regDbgContext(std::move(dbgContext));

	dbgExecutor->wait();
	return true;
}
