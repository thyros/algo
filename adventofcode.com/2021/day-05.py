from typing import DefaultDict

SAMPLE = [
    "0,9 -> 5,9",
    "8,0 -> 0,8",
    "9,4 -> 3,4",
    "2,2 -> 2,1",
    "7,0 -> 7,4",
    "6,4 -> 2,0",
    "0,9 -> 2,9",
    "3,4 -> 1,4",
    "0,0 -> 8,8",
    "5,5 -> 8,2"]

with open("day-05.input", "r") as fin:
    DATA = fin.readlines()
    DATA = [d.strip() for d in DATA]

def parse(data):
    lines = []
    for line in data:
        start, end = line.split(" -> ")
        x1, y1 = start.split(",")
        x2, y2 = end.split(",")

        lines.append([(int(x1), int(y1)), (int(x2),int(y2))])

    return lines

def part_1(lines):
    field = DefaultDict(lambda: 0)

    for start, end in lines:
        if start[0] == end[0]:              # horizontal line
            a = min(start[1], end[1])
            b = max(start[1], end[1]) + 1
            for y in range(a, b):
                field[(start[0], y)] += 1
        elif start[1] == end[1]:            # vertical line
            a = min(start[0], end[0])
            b = max(start[0], end[0]) + 1
            for x in range(a, b):
                field[(x, start[1])] += 1

    return sum(1 for v in field.values() if v > 1)

def direction(a: int, b: int):
    if a < b: 
        return 1
    elif a > b:
        return -1
    return 0

def part_2(lines):
    field = DefaultDict(lambda: 0)

    for start, end in lines:
        l = max(abs(start[0] - end[0]), abs(start[1] - end[1])) + 1
        dx = direction(start[0], end[0])
        dy = direction(start[1], end[1])

        for i in range(l):
            field[(start[0] + i * dx, start[1] + i * dy)] += 1

    return sum(1 for v in field.values() if v > 1)

lines = parse(DATA)

dangerous_areas = part_1(lines)
print(f"{dangerous_areas}")
dangerous_areas = part_2(lines)
print(f"{dangerous_areas}")