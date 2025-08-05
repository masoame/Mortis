#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgContextControl : DbgContext
	{
		DbgContextControl() = delete;
	public:
		template<typename T>
		DbgContextControl& regHook(std::size_t parameterPos,CallingConvention callingConvention = X64_CALL) {
			
			return *this;
		}

		DbgContextControl& regHookString(std::size_t parameterPos, CodePage codePage, CallingConvention callingConvention = X64_CALL, std::size_t StringLenPos = -1);


	};
}