#include<PE.h>
#include<iostream>

unsigned char magic[] =
{
  0x6B, 0x02, 0x66, 0x70, 0x44, 0x69, 0x7E, 0x6E, 0x43,
  0x4A, 0x78, 0x4A, 0x6D, 0x60, 0x56, 0x00, 0x51, 0x59,
  0x50, 0x43, 0x50, 0x51, 0x6D, 0x74, 0x02, 0x55, 0x50,
  0x52, 0x6E, 0x6F, 0x79, 0x40, 0x5D, 0x4B, 0x1E, 0x19,
  0x1C, 0x74, 0x03, 0x54, 0x07, 0x4C, 0x52, 0x6A, 0x60,
  0x50, 0x58, 0x40, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00
};


bool refreshPossibilities9x9(std::vector<std::vector<char>>& box, std::map<std::size_t, std::list<char>>& possibilities, std::optional<std::size_t>& min_pos)
{
	bool isChange;
	do {
		isChange = false;
		min_pos.reset();

		for (std::size_t row = 0; row < 9; row++) {
			for (std::size_t col = 0; col < 9; col++) {
				if (box[row][col] != 0) {
					if (possibilities.contains(row * 9 + col)) {
						possibilities.erase(row * 9 + col);
					}
					continue;
				}

				std::list<char>& used_numbers = possibilities[row * 9 + col];
				used_numbers = { 1,2,3,4,5,6,7,8,9 };
				// Check row
				for (std::size_t c = 0; c < 9; c++) {
					if (box[row][c] != 0) {
						std::erase_if(used_numbers, [&](std::size_t v) { return v == box[row][c]; });
					}
				}
				if (used_numbers.empty()) {
					return false;
				}

				// Check column
				for (std::size_t r = 0; r < 9; r++) {
					if (box[r][col] != 0) {
						std::erase_if(used_numbers, [&](std::size_t v) { return v == box[r][col]; });
					}
				}

				if (used_numbers.empty()) {
					return false;
				}

				// Check 3x3 box
				std::size_t box_start_row = (row / 3) * 3;
				std::size_t box_start_col = (col / 3) * 3;
				for (std::size_t r = box_start_row; r < box_start_row + 3; r++) {
					for (std::size_t c = box_start_col; c < box_start_col + 3; c++) {
						if (box[r][c] != 0) {
							std::erase_if(used_numbers, [&](std::size_t v) { return v == box[r][c]; });
						}
					}
				}

				if (used_numbers.empty()) {
					return false;
				}

				auto len = used_numbers.size();
				if (len == 1) {
					box[row][col] = static_cast<unsigned char>(*used_numbers.begin());
					possibilities.erase(row * 9 + col);
					isChange = true;
					continue;
				}

				if (min_pos.has_value() == false) {
					min_pos = row * 9 + col;
				}
				else {
					min_pos = (len < possibilities[min_pos.value()].size()) ? row * 9 + col : min_pos;
				}
			}
		}

	} while (isChange);

	return true;
}

bool SudokuSolution9x9(std::vector<std::vector<char>>& box, std::map<std::size_t, std::list<char>> possibilities)
{
	std::optional<std::size_t> min_pos;
	if (refreshPossibilities9x9(box, possibilities, min_pos) == false) {
		return false;
	}
	if (min_pos.has_value() == false) {
		return true;
	}

	for (const auto& num : possibilities[min_pos.value()]) {
		auto orgin_box = box;
		box[min_pos.value() / 9][min_pos.value() % 9] = static_cast<unsigned char>(num);
		if (SudokuSolution9x9(box, possibilities)) {
			return true;
		}
		box = std::move(orgin_box);
	}
	return false;
}

int main()
{
	std::vector<std::vector<char>> box = {
			{6, 5, 0, 2, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 1, 0, 0, 0, 4},
			{0, 3, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 3, 0, 0, 5, 0, 0},
			{7, 0, 8, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 6, 0, 0, 0, 0, 0},
			{1, 0, 0, 0, 7, 4, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 2, 3, 0},
			{0, 0, 0, 0, 0, 0, 0, 5, 0}
	};

	char ppbox[9][9] =
	{
	  0x00, 0x00, 0x05, 0x00, 0x00, 0x04, 0x03, 0x06, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x02, 0x04, 0x00, 0x04,
	  0x09, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x06,
	  0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x07,
	  0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x03, 0x00, 0x00, 0x00,
	  0x05, 0x00, 0x09, 0x00, 0x02, 0x00, 0x00, 0x05, 0x00, 0x07,
	  0x00, 0x00, 0x09, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08,
	  0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	  0x06
	};

	auto view = std::views::zip(box | std::views::join, ppbox | std::views::join);

	for (auto [a, b] : view) {
		a = b;
	}


	auto orginBox = box;
	for (auto& arr : box) {
		for (auto v : arr) {
			std::cout << static_cast<int>(v) << ",";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;

	std::map<std::size_t, std::list<char>> possibilities;

	SudokuSolution9x9(box, possibilities);
	for (auto& arr : box) {
		for (auto v : arr) {
			std::cout << static_cast<int>(v) << ",";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (auto& arr : box) {
		for (auto v : arr) {
			std::cout << static_cast<int>(v);
		}
	}
	std::cout << std::endl;

	std::vector<char> orginInput;
	auto pairView = std::views::zip(orginBox | std::views::join, box | std::views::join);
	for (const auto& [orginChar, resChar] : pairView) {
		if (orginChar == 0) {
			orginInput.push_back(resChar + '0');
		}
	}

	std::string flag;

	for (int i = 0; i != 49; ++i) {
		flag.push_back(orginInput[i] ^ magic[i]);

	}
	std::cout << "moectf{" << flag << '}' << std::endl;
}