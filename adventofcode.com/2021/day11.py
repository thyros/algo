SAMPLE = ['5483143223',
          '2745854711',
          '5264556173',
          '6141336146',
          '6357385478',
          '4167524645',
          '2176841721',
          '6882881134',
          '4846848554',
          '5283751526']

MINI_SAMPLE = ['11111',
               '19991',
               '19191',
               '19991',
               '11111']

with open('day11.input', 'r') as fin:
    DATA=[line.strip() for line in  fin.readlines()]
    
def parse(data):
    octopuses = {}
    height = len(data)
    width = len(data[0])
    for y in range(height):
        for x in range(width):
            octopuses[(x, y)] = int(data[y][x])

    return octopuses, width, height


def get_neighbours(x, y, width, height):
    neighbours = []
    for b in range(y - 1, y + 2):
        for a in range(x - 1, x + 2):
            if a >= 0 and b >= 0 and a < width and b < height:
                neighbours.append((a, b))
    neighbours.remove((x, y))

    return neighbours


def simulate(octopuses, width, height):
    octopuses = {k: v+1 for (k, v) in octopuses.items()}

    to_flash = [k for (k, v) in octopuses.items() if v > 9]
    flashed = []

    while to_flash:
        octopus = to_flash.pop()

        neighbours = get_neighbours(octopus[0], octopus[1], width, height)
        for n in neighbours:
            if n not in flashed and n not in to_flash:
                octopuses[n] += 1
                if octopuses[n] > 9:
                    to_flash.append(n)
                    
        flashed.append(octopus)

    for octopus in flashed:
        octopuses[octopus] = 0

    return octopuses, len(flashed)


def part_1(octopuses, width, height, steps):
    total_flashes = 0
    for i in range(steps):
        octopuses, flashes = simulate(octopuses, width, height)
        total_flashes += flashes

    return total_flashes

def part_2(octopuses, width, height):
    step = 0
    while True:
        step += 1
        octopuses, flashes = simulate(octopuses, width, height)
        if flashes == width * height:
            return step

data, width, height = parse(DATA)
total_flashes = part_1(data, width, height, 100)
print(total_flashes)

sync_flash_at = part_2(data, width, height)
print(sync_flash_at)