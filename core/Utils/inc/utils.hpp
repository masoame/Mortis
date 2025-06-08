#pragma once

#ifdef _WIN32
#include<Windows.h>
#include<tlhelp32.h>
#else

#endif

#include<optional>
#include<vector>
#include<map>
#include<array>


#include<base_api.hpp>
#include<base_types.hpp>

#include<ScopeHandle.hpp>
#include<bounded_queue.hpp>

namespace os
{
	extern std::optional<std::wstring> GetModuleFileNameW(HMODULE module = NULL);
}