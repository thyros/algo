import statistics

SAMPLE="16,1,2,0,4,2,7,1,2,14"

with open('day07.input', 'r') as fin:
    DATA=fin.read().strip()

def parse(data):
    return [int(x) for x in data.split(',')]

def part_1(data):
    median = int(statistics.median(data))
    print(median)

    fuel_used = [abs(median - x) for x in data]
    return sum(fuel_used)

def move_cost(a: int, b: int) -> int:
    m = min(a, b)
    a -= m
    b -= m
    return sum([x for x in range(1, max(a, b) + 1)])

def part_2(data):
    cheapest = 1000000000000
    for i in range(len(data)):
        fuel_used = [move_cost(i, x) for x in data]
        s = sum(fuel_used)
        if s < cheapest:
            cheapest = s

    return cheapest


crabs = parse(DATA)
print(part_2(crabs))