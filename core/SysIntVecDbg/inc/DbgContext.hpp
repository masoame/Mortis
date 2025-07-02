#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter;

	class DebugKey {
	public:
		DWORD _dw_exception_code;
		PVOID _fp_exception_address;

		auto operator<=>(const DebugKey&) const = default;
	};

	struct DebugContext : public DebugKey 
	{
		PE::INT_TYPE _int_code;
	private:
		CONTEXT _ctx;
		std::weak_ptr<DbgExecuter> _dbg_executer;
	protected:
		BYTE _origin_code;
		std::function<void()> _call_exception_handler;

		bool continueDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool stopDebug(const ScopeHandle<>& hProcess) const noexcept;

		bool saveOrginCode(const ScopeHandle<>& hProcess)noexcept;
		bool setIntCode(PE::INT_TYPE int_code) noexcept;
	public:
		DebugContext(PVOID fp_exception_address, DWORD _dw_exception_code = EXCEPTION_BREAKPOINT, PE::INT_TYPE _int_code = PE::INT_TYPE::INT3);
		DebugContext() = default;

		bool getThreadContext(const ScopeHandle<>& hThread,DWORD contextFlags = CONTEXT_ALL);
		bool setThreadContext(const ScopeHandle<>& hThread) const;

		void recoverRegisterRIP() noexcept;

		bool bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept;
		bool startDebug() noexcept;

		auto refreshThreadContext() 
			-> ScopeHandle<>;
		bool applyThreadContext(ScopeHandle<>&& threadHandle);

		void regExceptionCallBack(std::function<void(DebugContext& )> callBackFunc);
		bool exceptionCallBack() const;
	};
}
