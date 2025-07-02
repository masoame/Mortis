#include "ContextView.h"


using namespace Mortis::PE;

auto ContextView::getArg(std::size_t location, std::function<std::any(DWORD64)> callToConvertType, CallConventions callconventions)
	-> std::any
{
	location;
	callconventions;
	return callToConvertType(location);
}
