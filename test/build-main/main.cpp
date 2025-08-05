#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;


int main() {

	std::string_view test_name = "测试程序";
	std::ofstream test_file(L"test.txt", std::ios::binary);
	if (!test_file) {
		std::cerr << "Failed to create file: " << test_name << std::endl;
		return 1;
	}
	test_file.write(test_name.data(), test_name.size());
	test_file.close();



	return 0;
}
