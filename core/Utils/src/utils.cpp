#include <utils.hpp>

std::wstring os::GetModuleFileNameW(HMODULE module /* = NULL*/)
{
	std::wstring buffer(MAX_PATH, L'\0');
	if (GetModuleFileNameW(module, buffer.data(), MAX_PATH)) {
		return buffer;
	}
	return {};
}
