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

	auto dbg_context = std::make_unique<DebugContext>(&WriteFile);

	dbg_context->regExceptionCallBack(
		[](const DebugContext & ctx) {
			ctx;
			static int count = 0;
			spdlog::info("count: {}", ++count);
		}
	);
	auto dbg_executor = std::make_shared<DbgExecuter>(process_entry->th32ProcessID);
	dbg_executor->regDbgContext(std::move(dbg_context));

	dbg_executor->wait();
	return true;
}
