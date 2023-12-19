#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

struct Seat
{
	int row;
	int column;
};

Seat ParseSeat(std::string_view text)
{
	const std::string reversed(crbegin(text), crend(text));
	const int column = [&reversed]() -> int {
		int result = 0;
		int pow = 1;
		for (int i = 0; i < 3; ++i)
		{
			if (reversed[i] == 'R')
			{
				result += pow;
			}
			pow *= 2;
		}
		return result;
	}();

	const int row = [&reversed]() -> int {
		int result = 0;
		int pow = 1;
		for (int i = 3; i <= 10; ++i)
		{
			if (reversed[i] == 'B')
			{
				result += pow;
			}
			pow *= 2;
		}
		return result;
	}();

	return Seat{row, column};
}

int GetSeatId(const Seat& seat)
{
	const int seatId = seat.row * 8 + seat.column;
	return seatId;
}

int main()
{
	std::ifstream ifstr("day05.input");

	std::vector<int> seatIds = [&ifstr]() {
		std::vector<int> result;
		std::string seatText;
		while (ifstr >> seatText)
		{
			const Seat seat = ParseSeat(seatText);
			const int seatId = GetSeatId(seat);
			result.push_back(seatId);
		}
		return result;
	}();

	std::sort(begin(seatIds), end(seatIds));

	for (size_t i = 0; i < seatIds.size() - 1; ++i)
	{
		if (seatIds[i] + 1 != seatIds[i + 1])
		{
			std::cout << "Found Seat: " << seatIds[i + 1] << std::endl;
			return seatIds[i + 1];
		}
	}
}
