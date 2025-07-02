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

	auto process_entry = SearchProcess(L"notepad.exe");
	if (process_entry == nullptr) {
		std::cout << "Process not found" << std::endl;
		return -1;
	}

	auto dbgContext = std::make_unique<DebugContext>();
	dbgContext->_fp_exception_address = WriteFile;
	dbgContext->_dw_exception_code = EXCEPTION_BREAKPOINT;
	dbgContext->_int_code = INT_TYPE::INT3;
	dbgContext->regExceptionCallBack(
		[](DebugContext& ) {
			static int count = 0;
			spdlog::info("count: {}", ++count);
		}
	);
	auto dbgExecutor = std::make_shared<DbgExecuter>(process_entry->th32ProcessID);
	dbgExecutor->regDbgContext(std::move(dbgContext));

	dbgExecutor->wait();
	return true;
}
