from functools import reduce

SAMPLE = ["6,10",
          "0,14",
          "9,10",
          "0,3",
          "10,4",
          "4,11",
          "6,0",
          "6,12",
          "4,1",
          "0,13",
          "10,12",
          "3,4",
          "3,0",
          "8,4",
          "1,10",
          "2,14",
          "8,10",
          "9,0",
          "",
          "fold along y=7",
          "fold along x=5"]

with open('day-13.input', 'r') as fin:
    DATA = [line.strip() for line in fin.readlines()]

def parse(data):
    dots = []
    folds = []
    for line in data:
        if line:
            if line.startswith("fold"):
                line = line[len("fold along "):]
                l, v = line.split('=')
                if l == 'y':
                    folds.append((0, int(v)))
                else:
                    folds.append((int(v), 0))
            else:
                x, y = line.split(',')
                dots.append((int(x), int(y)))
    return dots, folds


def fold(dots, f):
    def fold_horizontaly(dot_y, fold_y):
        new_y = fold_y - (dot_y - fold_y) if dot_y > fold_y else dot_y
        return new_y
    def fold_verticaly(dot_x, fold_x):
        new_x = fold_x - (dot_x - fold_x) if dot_x > fold_x else dot_x
        return new_x

    if f[0] != 0:
        dots = {(fold_verticaly(x, f[0]), y) for (x,y) in dots}
    else:
        dots = {(x, fold_horizontaly(y, f[1])) for (x,y) in dots}

    return dots

def part_1(dots, folds):
    return fold(dots, folds[0])

def part_2(dots, folds):
    for f in folds:
        dots = fold(dots, f)

    width, height = reduce(lambda a, b: (max(a[0], b[0]), max(a[1], b[1])), dots, (0,0))
    
    page = []
    for h in range(height+1):
        page.append(['.'] * (width + 1))

    for d in dots:
        page[d[1]][d[0]] = 'X'

    return page

dots, folds = parse(DATA)

dots = part_1(dots, folds)
print(len(dots))

page = part_2(dots, folds)
for p in page:
    print(p)
