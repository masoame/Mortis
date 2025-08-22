#include<PE.h>
#include<iostream>

int main() {
	
	int num = 0;
	std::cin >> num;

	try{
		switch (num)
		{
		case 0:
			throw std::runtime_error("test error");
		case 1:
			throw 67;
		case 2: 
			throw "test c style error";

		default:
			break;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << std::current_exception().operator bool() << std::endl;
	}
}
