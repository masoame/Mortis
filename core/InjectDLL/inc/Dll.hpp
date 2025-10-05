#include<PE.hpp>

namespace Mortis
{
	template<typename DLLMaker>
	class BaseDLL : public Mortis::Singleton<DLLMaker>
	{
	protected:
		bool bRet = true;

		void Initialize(const HMODULE& hModule, PCONTEXT pCtx) {
			bRet = static_cast<DLLMaker*>(this)->Initialize(hModule, pCtx);
		}
		void Uninitialize(const HMODULE& hModule, PCONTEXT pCtx) {
			bRet = static_cast<DLLMaker*>(this)->Uninitialize(hModule, pCtx);
		}
		void Listen_Thread_Create(const HMODULE& hModule) {
			bRet = static_cast<DLLMaker*>(this)->Listen_Thread_Create(hModule);
		}
		void Listen_Thread_Destroy(const HMODULE& hModule) {
			bRet = static_cast<DLLMaker*>(this)->Listen_Thread_Destroy(hModule);
		}
	};
}

#define MAKE_DLL(DLLClass) \
using namespace Mortis;\
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) \
{\
	switch (ul_reason_for_call)\
	{\
	case DLL_PROCESS_ATTACH:\
		DLLClass::Instance().Initialize(hModule,reinterpret_cast<PCONTEXT>(lpReserved));\
		break;\
	case DLL_THREAD_ATTACH:\
		DLLClass::Instance().Listen_Thread_Create(hModule);\
		break;\
	case DLL_THREAD_DETACH:\
		DLLClass::Instance().Listen_Thread_Destroy(hModule);\
		break;\
	case DLL_PROCESS_DETACH:\
		DLLClass::Instance().Uninitialize(hModule,reinterpret_cast<PCONTEXT>(lpReserved));\
		break;\
	}\
	return static_cast<BOOL>(DLLClass::Instance().bRet);\
}