#include<DbgExecuter.hpp>
#include <iostream>

using namespace Mortis;
using namespace Mortis::PE;
using namespace Mortis::SysIntVecDbg;


using namespace std::chrono_literals;
int main()
{
	system("chcp 65001");

	const auto process_entry = ProcessInfoMap(PROCESS_ID)[(DWORD)22332];
	if (process_entry.is_multiple()) {
		spdlog::info("发现多个进程: {}", process_entry.get_multiple().size());
		return -1;
	}
	const auto& process_entry_single = process_entry.get_single();

	auto dbg_context = std::make_unique<DebugContext>(&WriteFile);

	dbg_context->regExceptionCallBack(
		[](const DebugContext & ctx) {
			ctx;
			static int count = 0;
			spdlog::info("count: {}", ++count);
		}
	);
	auto dbg_executor = std::make_shared<DbgExecuter>(process_entry_single.th32ProcessID);
	dbg_executor->regDbgContext(std::move(dbg_context));

	dbg_executor->wait();
	return 0;
}
