import re
from typing import NamedTuple

SAMPLE = "target area: x=20..30, y=-10..-5"

with open('day17.input', 'r') as fin:
    DATA = fin.read()
    
class TargetArea(NamedTuple):
    x1: int = 0
    y1: int = 0
    x2: int = 0
    y2: int = 0

class MaxY():
    max_y: int = 0
    
    def test(self, _: int, y: int):
        self.max_y = max(self.max_y, y)
        
class Hit():
    hit: bool = False
    target_area: TargetArea
    
    def __init__(self, target_area: TargetArea):
        self.target_area = target_area
    
    def test(self, x: int, y: int):
        self.hit = is_inside(x, y, self.target_area)

def parse(data):
    pattern = "target area: x=(?P<x1>[-+]?\d+)..(?P<x2>[-+]?\d+), y=(?P<y1>[-+]?\d+)..(?P<y2>[-+]?\d+)"
    matches = re.match(pattern, data)
    x1 = int(matches.group('x1'))
    x2 = int(matches.group('x2'))
    y1 = int(matches.group('y1'))
    y2 = int(matches.group('y2'))
    return TargetArea(x1 = x1, y1=y1, x2=x2, y2=y2)

def is_inside(x:int, y:int, target_area: TargetArea):
    return target_area.x1 <= x <= target_area.x2 and target_area.y1 <= y <= target_area.y2

def simulate(vx, vy, target_area: TargetArea, monitor):
    px = py = 0

    for _ in range(2 * abs(target_area.y1) + 2):
        px += vx
        py += vy
        
        monitor.test(px, py)
        
        vx = max(vx - 1, 0)
        vy -= 1
        
        if is_inside(px, py, target_area):
            break
        if px > target_area.x2 or py < target_area.y1:
            break

def part_1(target_area: TargetArea):
    max_y = 0
    for y in range(target_area.y1, abs(target_area.y1)):
        for x in range(1, target_area.x2 + 1):
            max_y_monitor = MaxY()
            simulate(x, y, target_area, max_y_monitor)
            max_y = max(max_y, max_y_monitor.max_y)

    return max_y

def part_2(target_area: TargetArea):
    count = 0
    for y in range(target_area.y1, abs(target_area.y1)):
        for x in range(1, target_area.x2 + 1):
            hit_monitor = Hit(target_area)
            simulate(x, y, target_area, hit_monitor)
            
            if hit_monitor.hit:
                count += 1
    return count

target_area = parse(DATA)
print(part_1(target_area))
print(part_2(target_area))