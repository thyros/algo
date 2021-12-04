SAMPLE = [
    "00100",
    "11110",
    "10110",
    "10111",
    "10101",
    "01111",
    "00111",
    "11100",
    "10000",
    "11001",
    "00010",
    "01010"]

with open("day-03.input", "r") as fin:
    DATA = fin.readlines()
    DATA = [d.strip() for d in DATA]

def to_most_common_bits(data):
    length = len(data[0])
    output = [0] * length
    for line in data:
        for i, v in enumerate(line):
            output[i] += 1 if v == '1' else -1
    return ['1' if v >= 0 else '0' for v in output]


def to_number(data):
    number = 0
    for v in data:
        number = number * 2 + (1 if v == '1' else 0)
    return number


def part_1(data):
    most_common_bits = to_most_common_bits(data)
    length = len(most_common_bits)
    gamma = to_number(most_common_bits)
    epsilon = pow(2, length) - 1 - gamma
    return gamma, epsilon

def keep(data, i, b):
    y = 0
    while y < len(data) and len(data) > 1:
        number = data[y]
        n = number[i]
        if n == b:
            y += 1
        else:
            del data[y]

    return data

def part_2(data):
    length = len(data)

    oxygen_data = data.copy()
    for i in range(length):
        most_common_bits = to_most_common_bits(oxygen_data)
        oxygen_data = keep(oxygen_data.copy(), i, most_common_bits[i])
        if len(oxygen_data) == 1:
            break
    oxygen_rating = to_number(oxygen_data[0])

    co2_data = data.copy()
    for i in range(length):
        most_common_bits = to_most_common_bits(co2_data)
        co2_data = keep(co2_data.copy(), i, '0' if most_common_bits[i] == '1' else '1')
        if len(co2_data) == 1:
            break
    co2_rating = to_number(co2_data[0])

    return oxygen_rating, co2_rating

gamma, epsilon = part_1(DATA)
print(f"{gamma} {epsilon} {gamma * epsilon}")
oxygen_rating, co2_rating = part_2(DATA)
print(f"{oxygen_rating} {co2_rating} {oxygen_rating * co2_rating}")
