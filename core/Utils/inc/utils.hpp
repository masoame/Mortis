#pragma once

#ifdef _WIN32
#include<Windows.h>
#include<tlhelp32.h>
#else

#endif

#include<mutex>
#include<deque>
#include<optional>
#include<vector>
#include<set>
#include<map>
#include<array>
#include<chrono>
#include<thread>
#include<locale>
#include<codecvt>
#include<ranges>
#include<algorithm>
#include<string>
#include<functional>
#include<any>
#include<future>
#include<format>
#include<variant>
#include<filesystem>


#include<spdlog/spdlog.h>


#include<base_concept.hpp>
#include<base_api.hpp>
#include<base_types.hpp>

#include<ReflFunc.hpp>
#include<ConvertString.hpp>
#include<ScopeHandle.hpp>
#include<ScopeWrapper.hpp>
#include<bounded_queue.hpp>

namespace os
{
	std::wstring GetModuleFileNameW(HMODULE module = NULL);
}
