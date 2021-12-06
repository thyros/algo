import cProfile
from typing import Deque

SAMPLE = "3,4,3,1,2"

with open("day-06.input", "r") as fin:
    DATA = fin.read().strip()

def parse(fish):
    return sorted([int(x) for x in fish.split(',')])

def part_1_pop(fish, days_count):
    population = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    for f in fish:
        population[f] += 1

    for i in range(days_count):
        new_borns = population.pop(0)

        population[6] += new_borns
        population.append(new_borns)

    return sum(population)

def part_1_rotate(fish, days_count):
    population = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    for f in fish:
        population[f] += 1

    for i in range(days_count):
        new_borns = population[0]

        for j in range(1,len(population)):
           population[j-1] = population[j]

        population[6] += new_borns
        population[8] = new_borns

    return sum(population)

def part_1_deque(fish, days_count):
    counts = Deque([fish.count(i) for i in range(9)])

    for i in range(days_count):
        counts.rotate(-1)
        counts[6] += counts[8]

    return sum(counts)

fish = parse(DATA)

cProfile.run('part_1_pop(fish, 1000000)')
cProfile.run('part_1_rotate(fish, 1000000)')
cProfile.run('part_1_deque(fish, 1000000)')

count = part_1_rotate(fish, 256)
print(count)
