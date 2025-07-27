#include<DbgContext.hpp>

namespace Mortis::SysIntVecDbg
{
	class DbgContextControl : DbgContext
	{
		using DbgContext::DbgContext;
	protected:

	public:

		template<typename T>
		DbgContextControl& regHook() {
		
		}

		DbgContextControl& regHookString();


	};
}