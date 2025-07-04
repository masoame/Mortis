#include<windows.h>

BOOL APIENTRY DllMain([[maybe_unused]] HMODULE hModule, [[maybe_unused]] DWORD  ul_reason_for_call, [[maybe_unused]] LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, 0, [](LPVOID)->DWORD
			{
				while (true)
				{
					if (IsDebuggerPresent()) {
						MessageBoxW(nullptr, L"Debugger Detected", L"Warning", MB_OK | MB_ICONWARNING);
						exit(-1);
					}
				}
			}, NULL, 0, NULL);
		break;
	case DLL_THREAD_ATTACH:

		break;
	case DLL_THREAD_DETACH:

		break;
	case DLL_PROCESS_DETACH:

		break;
	}
	return TRUE;
}
