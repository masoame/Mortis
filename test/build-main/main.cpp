#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;


int main() {

	std::vector<int> arr1 = { 0,1,2 };
	std::vector<int> arr2 = { 121,2,3 };
	std::copy(arr1.cbegin(), arr1.cend(), arr2.begin());
	std::for_each(arr2.cbegin(), arr2.cend(), [](int a) {
		std::cout << a << " ";
	});

	return 0;
}
