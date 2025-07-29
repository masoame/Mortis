#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgContextControl
	{
	private:
		DbgContext& _dbg_ctx;
	public:
		DbgContextControl(DbgContext& dbg_ctx);

		template<typename T>
		DbgContextControl& regHook(std::size_t regisisterPos,CallingConvention callingConvention = X64_CALL) {
			
			return *this;
		}

		DbgContextControl& regHookString();


	};
}