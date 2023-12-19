from typing import NamedTuple
import re

SAMPLE = ["[1,2]",
          "[[1,2],3]",
          "[9,[8,7]]",
          "[[1,9],[8,5]]",
          "[[[[1,2],[3,4]],[[5,6],[7,8]]],9]",
          "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]",
          "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]"]

NUMBER = re.compile('\d+')


class Number():
    def __init__(self, left=None, right=None) -> None:
        self.left = left
        self.right = right

    def __eq__(self, o: object) -> bool:
        return self.left == o.left and self.right == o.right


def parse(data: str, index: int = 0):
    def parse_number(data: str, index: int):
        c = data[index]
        if c == '[':
            left, index = parse_number(data, index + 1)  # skip '['
            right, index = parse_number(data, index + 1)  # skip ','
            return Number(left, right), index + 1

        m = NUMBER.search(data[index:])
        value = int(m[0])
        index += m.end()
        return value, index

    return parse_number(data, index)


def explode(Number, depth = 0):
    

n = explode(Number(Number(1, 2), 3), 5)
n = explode(Number(9, Number(8, 7)), 5)
n = explode(Number(Number(1, 9), Number(8, 5)), 5)
print(n)
# TEST_CASES = [('[2,3]', Number(2, 3)),
#               ('[[1,2],3]', Number(Number(1, 2), 3)),
#               ('[9,[8,7]]', Number(9, Number(8, 7))),
#               ('[[1,9],[8,5]]', Number(Number(1, 9), Number(8, 5)))]

# for input, expected in TEST_CASES:
#     number, _ = parse(input)
#     assert number == expected

# EXPLODE_TEST_CASES = [('[[[[[9,8],1],2],3],4]', '[[[[0,9],2],3],4]')
#                       ('[7,[6,[5,[4,[3,2]]]]]', '[7,[6,[5,[7,0]]]]'),
#                       ('[[6,[5,[4,[3,2]]]],1]', '[[6,[5,[7,0]]],3]'),
#                       ('[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]',
#                        '[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]'),
#                       ('[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]',
#                        '[[3,[2,[8,0]]],[9,[5,[7,0]]]]')
#                       ]
# for input, expected in EXPLODE_TEST_CASES:
#     left = parse(input)
#     right = parse(expected)

#     assert explode(left) == right
