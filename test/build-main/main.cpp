#include <MutualMap.hpp>

using namespace Mortis;
using namespace Mortis::PE;
int main()
{
	MutualMap<std::string, int, std::string> testMap;
	testMap.insert("values", 9, "key2");

	//testMap.find(9);

}
