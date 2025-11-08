#pragma once
#include<ScopeHandle.hpp>
#include<tuple>
namespace Mortis
{
	struct ScopeVirtualMemory
	{
		std::unique_ptr<void, std::function<BOOL(LPVOID)>> _scope;

		mutable LPVOID _mem_adress = nullptr;
		ScopeVirtualMemory(const ScopeHandle<>& hProcess, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect) {
			_mem_adress = VirtualAllocEx(hProcess, nullptr, dwSize, flAllocationType, flProtect);
			_scope = std::unique_ptr<void, std::function<BOOL(LPVOID)>>(_mem_adress,std::bind(VirtualFreeEx, hProcess.get(), std::placeholders::_1, dwSize, MEM_RELEASE));
		}
		ScopeVirtualMemory(ScopeVirtualMemory&& _scope_vir_memory) noexcept = default;

		ScopeVirtualMemory(const ScopeVirtualMemory&) = delete;

		operator LPVOID () const {
			return _mem_adress;
		}
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