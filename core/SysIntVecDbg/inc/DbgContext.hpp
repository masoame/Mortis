#pragma once
#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DbgExecuter;
	class DbgContextControl;

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

		std::vector<BYTE> _origin_code;
		std::vector<BYTE> _replace_code;
		std::function<void()> _call_exception_handler;

		bool exceptionCallBack();
	protected:
		bool bindDbgExecuter(const std::shared_ptr<DbgExecuter>& pDbgExecuter) noexcept;

		bool continueDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool stopDebug(const ScopeHandle<>& hProcess) const noexcept;
		bool startDebug() noexcept;

		bool saveOrginCode(const ScopeHandle<>& hProcess)noexcept;
		bool setCode(std::span<const BYTE> int_code) noexcept;

		void recoverRegisterRIP() noexcept;
		bool applyThreadContext(ScopeHandle<>&& threadHandle);

		bool getThreadContext(const ScopeHandle<>& hThread, DWORD contextFlags = CONTEXT_ALL);
		bool setThreadContext(const ScopeHandle<>& hThread, const CONTEXT & = {}) const;

		auto refreshThreadContext()
			-> ScopeHandle<>;
	public:
		DbgContext(PVOID fp_exception_address, DWORD _dw_exception_code = EXCEPTION_BREAKPOINT, std::span<const BYTE> replace_code = PE::INT3);
		DbgContext(PVOID fp_exception_address, std::span<const BYTE> replace_code);
		DbgContext() = default;

		void regExceptionCallBack(std::function<void(DbgContext& )> callBackFunc);

		std::optional<CONTEXT> tryGetContext() const;

		DbgContextControl control();
	};
}
