from functools import reduce

SAMPLE = ["2199943210",
          "3987894921",
          "9856789892",
          "8767896789",
          "9899965678"]

with open('day09.input', 'r') as fin:
    DATA=[line.strip() for line in  fin.readlines()]

def parse_data(data):
    grid = {}
    height = len(data)
    width = len(data[0])
    for y in range(0, height):
        for x in range(0, width):
            grid[(x, y)] = int(data[y][x])
            
    return grid, width, height

def get_neighbours(x, y, width, height):
    neighbours = []
    if x > 0:
        neighbours.append((x-1, y))
    if y > 0:
        neighbours.append((x, y-1))
    if x < width - 1:
        neighbours.append((x+1, y))
    if y < height - 1:
        neighbours.append((x, y+1))
    return neighbours

def find_low_points(grid, width, height):
    low_points = []
    for y in range(0, height):
        for x in range(0, width):
            low_point = grid[(x, y)]
            other_points = [grid[n] for n in get_neighbours(x, y, width, height)]
            if low_point < min(other_points):
                low_points.append((x, y))
    
    return low_points

def part_1(grid, width, height):
    low_points = find_low_points(grid, width, height)
    
    risks = [grid[low_point] + 1 for low_point in low_points]
    return sum(risks)

def find_basin(grid, width, height, low_point):
    basin = []
    points_to_check = [low_point]
    
    while len(points_to_check) > 0:
        point_to_check = points_to_check.pop()
        if grid[point_to_check] != 9 and point_to_check not in basin:
            basin.append(point_to_check)
            points_to_check += get_neighbours(point_to_check[0], point_to_check[1], width, height)
    
    return basin

def part_2(grid, width, height):
    low_points = find_low_points(grid, width, height)
    basins = [find_basin(grid, width, height, p) for p in low_points]
    areas = sorted([len(basin) for basin in basins], reverse=True)
    return reduce(lambda x,y: x*y, areas[:3])

grid, width, height = parse_data(DATA)
risk_level = part_1(grid, width, height)

areas = part_2(grid, width, height)
print(areas)