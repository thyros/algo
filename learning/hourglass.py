INPUT = ["0 -4 -6 0 -7 -6",
         "-1 -2 -6 -8 -3 -1",
         "-8 -4 -2 -8 -8 -6",
         "-3 -1 -2 -5 -7 -4",
         "-3 -5 -3 -6 -6 -6",
         "-3 -6 0 -8 -6 -7"]


def sum(x, y, arr):
    offsets = [(0, 0), (1, 0), (2, 0),
                (1, 1),
                (0, 2), (1, 2), (2, 2)]
    s = 0
    for ox, oy in offsets:
        s += arr[y+oy][x+ox]
    return s

def hourglassSum(arr):
    res = sum(0, 0, arr)
    for y in range(4):
        for x in range(4):
            res = max(res, sum(x, y, arr))
            print(f"{x} {y} {res}")


def split_row(row):
    return [int(x) for x in row.split(' ')]

arr = [split_row(row) for row in INPUT]
print(arr)

hourglassSum(arr)
