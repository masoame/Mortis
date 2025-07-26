#pragma once
#include<base_template.hpp>
#include<tuple>
namespace Mortis
{
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

		constexpr ScopeExecutor(PurgeFunc&& func, Args&& ...args) :
			_func{ std::forward<PurgeFunc>(func) }, _args{ std::forward<Args>(args)... } {
		}

		constexpr ~ScopeExecutor() {
			std::apply(_func, _args);
		}
	};
	template<typename PurgeFunc, typename...Args>
	ScopeExecutor(PurgeFunc&&, Args&&...) -> ScopeExecutor<std::decay_t<PurgeFunc>, std::decay_t<Args>...>;

}