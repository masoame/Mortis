#include<PE.h>
#include<iostream>
using namespace Mortis;
using namespace Mortis::PE;


int main() {

	auto info_map = ProcessInfoMap(PROCESS_SZExeFile);

	for(const auto& info : info_map)
	{
		std::cout << "\nprocess name: \n" << get<std::string>(info.first) << "\nprocess_id : \n";

		if (std::holds_alternative<Mortis::PE::PROCESSENTRY32<char>>(info.second)) {
			std::cout << std::get<Mortis::PE::PROCESSENTRY32<char>>(info.second).th32ProcessID << std::endl;
		}
		else {
			for (const auto& value: std::get<std::vector<Mortis::PE::PROCESSENTRY32<char>>>(info.second)) {
				std::cout << value.th32ProcessID << " ";
			}
			std::cout << std::endl;
		}
	}

	std::cout << std::boolalpha << info_map.contains("notepad.exe") << std::endl;

	return 0;
}
