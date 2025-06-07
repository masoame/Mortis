#pragma once

#ifdef _WIN32
#include<Windows.h>
#include<tlhelp32.h>
#else

#endif

#include<optional>
#include<vector>
#include<map>

#include<ScopeHandle.hpp>
#include<bounded_queue.hpp>

namespace os
{
	inline std::optional<std::wstring> GetModuleFileNameW(HMODULE module = NULL)
	{
		std::wstring buffer(MAX_PATH, L'\0');
		if (GetModuleFileNameW(module, buffer.data(), MAX_PATH)) return buffer;
		return {};
	}
}