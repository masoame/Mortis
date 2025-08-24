#include<PE.h>
#include<iostream>

wchar_t keys[] = {
 0x3500, 0x6100, 0x6300, 0x3000, 0x2D00, 0x4C00, 0x6400, 0x7200, 0x4700, 0x6500, 0x7400, 0x0000
};

using Fn_H = char(*)(char);
int main() {

	std::wstring_view sv(keys);
	std::wcout << sv << std::endl;
	system("pause");
	return 0;
}