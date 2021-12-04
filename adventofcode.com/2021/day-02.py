SAMPLE = ["forward 5",
          "down 5",
          "forward 8",
          "up 3",
          "down 8",
          "forward 2"]

def to_command(line: str):
    d, l = line.strip().split(" ")
    return (d, int(l))

with open("day-02.input", "r") as fin:
    DATA = fin.readlines()

def part_1():
    return {
        "forward": lambda xya, l: (xya[0]+l, xya[1], xya[2]),
        "up": lambda xya, l: (xya[0], xya[1]-l, xya[2]),
        "down": lambda xya, l: (xya[0], xya[1]+l, xya[2])
    }

def part_2():
    return {
        "forward": lambda xya, l: (xya[0]+l, xya[1] + xya[2]*l, xya[2]),
        "up": lambda xya, l: (xya[0], xya[1], xya[2]-l),
        "down": lambda xya, l: (xya[0], xya[1], xya[2]+l)
    }

def execute(program, commands):
    d = (0,0,0)
    for command in commands:
        d = program[command[0]](d, command[1])
    return d

COMMANDS = [to_command(line) for line in DATA]

x, y, a = execute(part_1(), COMMANDS)
print(f"{x}, {y}, {x*y}")

x, y, a = execute(part_2(), COMMANDS)
print(f"{x}, {y}, {x*y}")