#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgContextControl : DbgContext
	{
		DbgContextControl() = delete;
	public:
		template<typename T>
		DbgContextControl& regHook(std::size_t regisisterPos,CallingConvention callingConvention = X64_CALL) {
			
			return *this;
		}

		DbgContextControl& regHookString();


	};
}