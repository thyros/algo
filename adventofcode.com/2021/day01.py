from typing import DefaultDict

def part_1(data):
    diffs = [j - i for i, j in zip(data, data[1:])]
    increased = sum(1 for x in diffs if x > 0)
    return increased

def part_2(data):
    sums = [a+b+c for a, b, c in zip(data, data[1:], data[2:])]
    diffs = [j - i for i, j in zip(sums, sums[1:])]
    increased = sum(1 for x in diffs if x > 0)
    return increased

with open("day01.input", "r") as fin:
    DATA = [int(x) for x in fin.readlines()]

print("Part 1: {}".format(part_1(DATA)))
print("Part 2: {}".format(part_2(DATA)))
