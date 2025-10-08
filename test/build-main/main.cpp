#include<PE.h>
#include <iostream>

#include <iostream>
#include <string>
#include <vector>

const std::string_view base64_chars ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(std::string_view in) {
	std::string out;

	int val = 0, valb = -6;
	for (unsigned char c : in) {
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			out.push_back(base64_chars[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) {
		out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
	}
	while (out.size() % 4) {
		out.push_back('=');
	}
	return out;
}

// 修正的 EnRangesXor (加密)
std::string EnRangesXor(std::string_view str)
{
	if (str.empty()) return "";

	std::string res{ str.data(), str.size() };

	for (size_t i = 0; i < str.size() - 1; ++i) {
		res[i] = str[i] ^ str[i + 1];
	}
	return res;
}

std::string DeRangesXor(std::string_view str)
{
	if (str.empty()) return "";
	std::string res{ str.data(), str.size() };

	for (size_t i = str.size() - 2; i != (size_t)-1; --i) {

		res[i] = res[i + 1] ^ str[i];
	}
	return res;
}

char flag[] = { 43, 56, 26, 14, 35, 29, 109, 96, 16, 126, 64, 40, 6, 98, 94, 63, 1, 4, 96, 87, 55, 108, 0, 61 ,0 };

int main()
{
	std::string input;
	std::cin >> input;

	if (EnRangesXor(base64_encode(input)) == std::string_view{ flag, sizeof(flag) -1 }) {
		spdlog::info("yes you find!!!");
	}
	else {
		spdlog::error("invalid entry Please try again...");
	}
    return 0;
}