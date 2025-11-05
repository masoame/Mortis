#include <utils.hpp>

#include <iostream>

struct TestData
{
	int i = 0;
	int j = 0;
};

int main()
{
	std::map<int, TestData> m;

	for (int i = 0; i != 20; ++i) {
		m.emplace(i, TestData{ i,i * 2 });
	}

	auto view_values =  m | std::views::values | std::views::filter([](const TestData& v) {
		return v.j % 3 == 0;
	});

	for (const auto& [key,valus] : m){
		std::cout << std::format("key:{},value:{{{},{}}}\n", key, valus.i, valus.j);
		std::cout << &valus << std::endl;
	}

	for (const auto& valus : view_values){
		std::cout << std::format("value:{{{},{}}}\n",  valus.i, valus.j);
		std::cout << &valus << std::endl;
	}

}// Dear ImGui: standalone example application for DirectX 11
