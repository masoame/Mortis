#pragma once

#ifdef _WIN32

#include <windows.h>

inline constexpr const BYTE INT3 = 0xCC;
inline constexpr const BYTE NOP = 0x90;
inline constexpr const BYTE RET = 0xC3;

#endif

template<typename AddressType = LPVOID>
inline auto MakeAddress(auto address, auto offset) noexcept -> LPVOID {
	return reinterpret_cast<AddressType>(reinterpret_cast<SIZE_T>(address) + offset);
};


