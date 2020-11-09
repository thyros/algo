/*
    Description:
    In this little assignment you are given a string of space separated numbers, and have to return the highest and lowest number.

    Example:

    highAndLow("1 2 3 4 5");  // return "5 1"
    highAndLow("1 2 -3 4 5"); // return "5 -3"
    highAndLow("1 9 3 4 -5"); // return "9 -5"
    Notes:

    All numbers are valid Int32, no need to validate them.
    There will always be at least one number in the input string.
    Output string must be two numbers separated by a single space, and highest number is first.
*/

std::string highAndLow(const std::string& numbers){
    std::istringstream iss(numbers);

    const auto [min, max] = std::minmax_element(std::istream_iterator<int>{iss},
                                                std::istream_iterator<int>{});

    return std::to_string(*max) + " " + std::to_string(*min);
}