#include "Dll.hpp"

extern BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

namespace Mortis
{
	class DLLMaker :public BaseDLL<DLLMaker>
	{
		friend class BaseDLL<DLLMaker>;
		friend BOOL APIENTRY ::DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
	protected:
		bool Initialize(const HMODULE& hModule, PCONTEXT pCtx) {
			hModule, pCtx;
			return true;
		}
		bool Uninitialize(const HMODULE& hModule, PCONTEXT pCtx) {
			hModule, pCtx;
			return true;
		}
		bool Listen_Thread_Create(const HMODULE& hModule) {
			hModule;
			return true;
		}
		bool Listen_Thread_Destroy(const HMODULE& hModule) {
			hModule;
			return true;
		}
	};
}

MAKE_DLL(DLLMaker);
