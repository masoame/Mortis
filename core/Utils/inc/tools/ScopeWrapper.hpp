#pragma once
#include<ScopeHandle.hpp>
#include<tuple>
namespace Mortis
{
	struct ScopeVirtualMemory
	{
		std::unique_ptr<void, std::function<BOOL(LPVOID)>> _scope;

		ScopeVirtualMemory(const ScopeHandle<>& hProcess ,LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType, DWORD flProtect){
			auto memAdress = VirtualAllocEx(hProcess, lpAddress, dwSize, flAllocationType, flProtect);
			_scope = std::unique_ptr<void, std::function<BOOL(LPVOID)>>(memAdress,
				std::bind(VirtualFreeEx, hProcess.get(), std::placeholders::_1, dwSize, flAllocationType));
		}
		ScopeVirtualMemory(ScopeVirtualMemory&& _scope_vir_memory) {
			_scope = std::move(_scope_vir_memory._scope);
		}

		ScopeVirtualMemory(const ScopeVirtualMemory&) = delete;
	};

	template<typename PurgeFunc, typename... Args>
		requires requires(PurgeFunc f, Args...args) { std::invoke(f, args...); }
	struct ScopeExecutor
	{
	protected:
		PurgeFunc _func;
		std::tuple<Args...> _args;
	public:
		ScopeExecutor(ScopeExecutor&&) = delete;
		ScopeExecutor(ScopeExecutor&) = delete;

		constexpr explicit ScopeExecutor(PurgeFunc&& func, Args&& ...args) :
			_func{ std::forward<PurgeFunc>(func) }, _args{ std::forward<Args>(args)... } {
		}

		constexpr ~ScopeExecutor() {
			std::apply(_func, _args);
		}
	};
	template<typename PurgeFunc, typename...Args>
	ScopeExecutor(PurgeFunc&&, Args&&...) -> ScopeExecutor<std::decay_t<PurgeFunc>, std::decay_t<Args>...>;

	//template<typename PurgeFunc, typename...Args>
	//	requires requires (BT::StaticFunctorWrapper<PurgeFunc>::type f, Args...args) { std::invoke(f, args...); }
	//class ScopeExecutor(Args... args) {

	//}

}