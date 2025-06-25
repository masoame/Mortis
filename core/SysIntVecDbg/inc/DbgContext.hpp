#include<PE.h>

namespace Mortis::SysIntVecDbg
{
	class DebugContext {
	public:
		
		auto operator<=>(const DebugContext&) const = default;
	};
}
