/*
    Create a function that returns the sum of the two lowest positive numbers given an array of minimum 4 positive integers. No floats or non-positive integers will be passed.

    For example, when an array is passed like [19, 5, 42, 2, 77], the output should be 7.

    [10, 343445353, 3453445, 3453545353453] should return 3453455.
*/

long sumTwoSmallestNumbers(std::vector<int> numbers)
{
    const auto [m1, m2] = 
        std::accumulate(cbegin(numbers), 
                        cend(numbers), 
                        std::make_pair(std::numeric_limits<long>::max(), std::numeric_limits<long>::max()),
                        [](auto acc, int current) -> std::pair<long, long>{
                            auto [a, b] = acc;
                            if (current < a) { b = a; a = current; }
                            else if (current < b) { b = current; }
                            return std::make_pair(a,b);
                        });
    
    return m1 + m2;
}