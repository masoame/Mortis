#pragma once

#ifdef _WIN32
#include <Winsock2.h>
#include<Windows.h>
#include<tlhelp32.h>
#else
#endif

#include<deque>
#include<queue>
#include<array>
#include<vector>
#include<set>
#include<map>
#include <list>
#include <ranges>

#include<chrono>
#include<locale>
#include<codecvt>
#include<algorithm>
#include<string>
#include<functional>

#include<format>
#include<any>
#include<variant>

#include<fstream>
#include<filesystem>

#include<optional>
#include<expected>

#include<thread>
#include<stop_token>
#include<mutex>
#include<future>

#include<spdlog/spdlog.h>
#include<nlohmann/json.hpp>
#include <magic_enum/magic_enum.hpp>
#include <blockingconcurrentqueue.h>
#include <sigslot/signal.hpp>

#include<base_concept.hpp>
#include<base_api.hpp>
#include<base_types.hpp>
#include<EnumWrapper.hpp>

#include<Singleton.hpp>
#include<ThreadPool.hpp>
#include<ReflFunc.hpp>
#include<ConvertString.hpp>
#include<ScopeHandle.hpp>
#include<ScopeWrapper.hpp>
#include<bounded_queue.hpp>


namespace os
{
	std::wstring GetModuleFileNameW(HMODULE module = NULL);
}
