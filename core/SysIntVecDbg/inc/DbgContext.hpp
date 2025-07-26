#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter;

	struct DbgKey {
		DWORD _dw_exception_code;
		PVOID _fp_exception_address;

		auto operator<=>(const DbgKey&) const = default;
	};

	struct DbgContext : public DbgKey 
	{
		friend class DbgExecuter;
	private:
		std::weak_ptr<DbgExecuter> _dbg_executer{};
		std::weak_ptr<CONTEXT> _thread_ctx{};
		PE::INT_TYPE _int_code;
		std::function<void()> _call_exception_handler;
		BYTE _origin_code;

		bool exceptionCallBack();
	protected:
		bool bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept;

		bool continueDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool stopDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool startDebug() noexcept;

		bool saveOrginCode(const ScopeHandle<>& hProcess)noexcept;
		bool setIntCode(PE::INT_TYPE int_code) noexcept;

		void recoverRegisterRIP() noexcept;
		bool applyThreadContext(ScopeHandle<>&& threadHandle);

		bool getThreadContext(const ScopeHandle<>& hThread, DWORD contextFlags = CONTEXT_ALL);
		bool setThreadContext(const ScopeHandle<>& hThread) const;

		auto refreshThreadContext()
			-> ScopeHandle<>;
	public:
		DbgContext(PVOID fp_exception_address, DWORD _dw_exception_code = EXCEPTION_BREAKPOINT, PE::INT_TYPE _int_code = PE::INT_TYPE::INT3);
		DbgContext() = default;

		void regExceptionCallBack(std::function<void(DbgContext& )> callBackFunc);
	};
}
