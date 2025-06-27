#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter;

	class DebugKey {
	public:
		DWORD _dw_exception_code;
		PVOID _fp_exception_address;
		//DebugKey() noexcept = default;
		auto operator<=>(const DebugKey&) const = default;
	};

	class DebugContext : public DebugKey {
	private:
		CONTEXT _ctx;
		std::weak_ptr<DbgExecuter> _dbgExecuter;
	protected:
		mutable BYTE _origin_code;
		mutable PE::INT_TYPE _int_code;

		std::function<void()> _callExceptionHandler;

		bool continueDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool stopDebug(const ScopeHandle<>& hProcess)const noexcept;

		bool saveOrginCode(const ScopeHandle<>& hProcess)noexcept;
		bool setIntCode(PE::INT_TYPE int_code) noexcept;
	public:
		bool getThreadContext(const ScopeHandle<>& hThread,DWORD contextFlags = CONTEXT_ALL);
		bool setThreadContext(const ScopeHandle<>& hThread) const;

		void recoverRegisterRIP() noexcept;

		bool bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept;
		bool startDebug() noexcept;


		auto refreshThreadContext() 
			-> ScopeHandle<>;
		bool applyThreadContext(ScopeHandle<>&& threadHandle);

		void exceptionCallBack();
	};
}
