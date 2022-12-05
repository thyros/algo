INPUTS = [("2 1 5 3 4", 3),
          ("2 5 1 3 4", None),
          ("5 1 2 3 7 8 6 4", None),
          ("1 2 5 3 7 8 6 4", 7),
          ("1 2 5 3 4 7 8 6", 4)]


def count_swaps(input):
    print(input)
    for i in range(len(input)):
        bribes = input[i] - i - 1
        if bribes > 2:
            return None

    swaps = 0
    while True:
        swapped = False
        for i in range(1, len(input)):
            if input[i-1] > input[i]:
                input[i-1], input[i] = input[i], input[i-1]
                swapped = True
                swaps = swaps + 1

        if not swapped:
            break

    return swaps


for input, expected_swaps in INPUTS:
    swaps = count_swaps([int(x) for x in input.split()])
    solved = swaps == expected_swaps
    print(f"{input} -> {swaps} {expected_swaps} {solved}")
