from functools import reduce

SAMPLE = [
    "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe",
    "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc",
    "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg",
    "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb",
    "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea",
    "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb",
    "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe",
    "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef",
    "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb",
    "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce"]

with open('day-08.input', 'r') as fin:
    DATA=fin.readlines()

def parse(data):
    a = [x.split(' | ') for x in data]
    b = [{"input": x[0].split(), "digits": x[1].split()} for x in a]
    return b


def part_1(data):
    s = 0
    for entry in data:
        s += len([x for x in entry["digits"] if len(x) == 2 or len(x) == 3 or len(x) == 4 or len(x) == 7])
    return s

def can_be_make_from(a, b):
    diff = set(a).difference(b)
    return len(diff) == 1


def decode(input):
    digits = {}

    one = [i for i in input if len(i) == 2][0]
    four = [i for i in input if len(i) == 4][0]
    seven = [i for i in input if len(i) == 3][0]
    eight = [i for i in input if len(i) == 7][0]

    five_digits_numbers = [i for i in input if len(i) == 5]
    three = [i for i in five_digits_numbers if one[0] in i and one[1] in i][0]
    rest_five_digits_numbers = [i for i in five_digits_numbers if i != three]

    six_digits_numbers = [i for i in input if len(i) == 6]
    six = [i for i in six_digits_numbers if one[0] not in i or one[1] not in i][0]
    rest_six_digits_numbers = [i for i in six_digits_numbers if i != six]

    if can_be_make_from(rest_six_digits_numbers[0], three):
        nine = rest_six_digits_numbers[0]
        zero = rest_six_digits_numbers[1]
    else:
        nine = rest_six_digits_numbers[1]
        zero = rest_six_digits_numbers[0]

    if (can_be_make_from(nine, rest_five_digits_numbers[0])):
        five = rest_five_digits_numbers[0]
        two = rest_five_digits_numbers[1]
    else:
        five = rest_five_digits_numbers[1]
        two = rest_five_digits_numbers[0]

    digits["".join(sorted(zero))] = 0
    digits["".join(sorted(one))] = 1
    digits["".join(sorted(two))] = 2
    digits["".join(sorted(three))] = 3
    digits["".join(sorted(four))] = 4
    digits["".join(sorted(five))] = 5
    digits["".join(sorted(six))] = 6
    digits["".join(sorted(seven))] = 7
    digits["".join(sorted(eight))] = 8
    digits["".join(sorted(nine))] = 9

    return digits

def convert(data, codes):
    numbers = [codes["".join(sorted(d))] for d in data]
    return reduce(lambda x, y: x * 10 + y, numbers, 0)

def convert_to_number(input, digits):
    codes = decode(input)
    value = convert(digits, codes)
    return value

def part_2(data):
    result = 0
    for d in data:
        result += convert_to_number(d["input"], d["digits"])
    return result

data = parse(DATA)
# print(data)

#print(part_1(data))

print(part_2(data))

# print(decode(["acedgfb", "cdfbe", "gcdfa", "fbcad", "dab", "cefabd", "cdfgeb", "eafb", "cagedb", "ab"]))
# print(convert_to_number(["acedgfb", "cdfbe", "gcdfa", "fbcad", "dab", "cefabd", "cdfgeb", "eafb", "cagedb", "ab"], ["cdfeb","fcadb","cdfeb","cdbaf"]))
# print(can_be_make_from("cefabd","fcabd")) # 9 out of 3
# print(can_be_make_from("cagebd", "fcabd")) # 0 out of 3
