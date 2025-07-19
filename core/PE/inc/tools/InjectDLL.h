#pragma once
#include <PE.h>


namespace Mortis::PE
{
	bool InjectDll(DWORD th32ProcessID, const wchar_t* dll);
}