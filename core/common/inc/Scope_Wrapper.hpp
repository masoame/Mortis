#pragma once
#include <type_traits>

namespace Mortis
{
	template<typename PurgeFunc, typename... Args>
		requires requires(PurgeFunc f, Args...args) { std::invoke(f, args...); }
	struct ScopeExecutor
	{
	protected:
		PurgeFunc _func;
		std::tuple<Args...> _args;

		bool _turnedOff = false;
	public:

		ScopeExecutor(ScopeExecutor&) = delete;
		ScopeExecutor(PurgeFunc&& func, Args&& ...args) : _func{ std::forward<PurgeFunc>(func) }, _args{ std::forward<Args>(args)... }
		{
		}

		void TurnOff() {
			_turnedOff = true;
		}

		~ScopeExecutor() {
			if (!_turnedOff) {
				std::apply(_func, _args);
			}
		}
	};
	template<typename PurgeFunc, typename...Args>
	ScopeExecutor(PurgeFunc&&, Args&&...) -> ScopeExecutor<std::decay_t<PurgeFunc>, std::decay_t<Args>...>;

	template<typename PurgeFunc>
	class ScopeWrapper
	{
		~ScopeWrapper() {  }
	};
	
	
}