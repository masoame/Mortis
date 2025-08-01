#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;

std::expected<int, std::string_view> sum(int a,int b) {
	if(a == 0 || b == 0) {
		return std::unexpected("a or b is zero");
	}
	return a + b ;
}

int main() {

	auto a = sum(6, 5).transform([](int a) {
		std::printf("sum is %d\n", a);
		return ;
		});

	return a.has_value();
}
