from functools import reduce

SAMPLE = ["[({(<(())[]>[[{[]{<()<>>",
          "[(()[<>])]({[<{<<[]>>(",
          "{([(<{}[<>[]}>{[]{[(<()>",
          "(((({<>}<{<{<>}{[]{[]{}",
          "[[<[([]))<([[{}[[()]]]",
          "[{[{({}]{}}([{[{{{}}([]",
          "{<[[]]>}<{[{[{[]{()[[[]",
          "[<(<(<(<{}))><([]([]()",
          "<{([([[(<>()){}]>(<<{{",
          "<{([{{}}[<[[[<>{}]]]>[]]"]

with open('day-10.input', 'r') as fin:
    DATA=[line.strip() for line in  fin.readlines()]

def find_incorrect_character(line):
    brackets = {'{': '}', '[': ']', '(': ')', '<': '>'}
    stack = [line[0]]

    for i in range(1, len(line)):
        a = line[i]
        if a in brackets.keys():
            stack.append(a)
        else:
            b = stack.pop()
            if brackets[b] != a:
                return a
    return None

def part_1(data):
    score = {')': 3,
              ']': 57,
              '}': 1197,
              '>': 25137}

    incorrect_characters = [find_incorrect_character(line) for line in data]
    values = [score[c] if c in score else 0 for c in incorrect_characters]

    return sum(values)

def find_open_brackets(line):
    brackets = {'{': '}', '[': ']', '(': ')', '<': '>'}
    stack = [line[0]]

    for i in range(1, len(line)):
        a = line[i]
        if a in brackets.keys():
            stack.append(a)
        else:
            b = stack.pop()
            if brackets[b] != a:
                return None
    return stack

def part_2(data):
    score = {'(': 1,
             '[': 2,
             '{': 3,
             '<': 4}

    def calculate_score(remaining_brackets):
        return reduce(lambda x, y: x * 5 + score[y], remaining_brackets, 0)

    open_brackets = [find_open_brackets(line) for line in data]
    remaining_brackets = [reversed(x) for x in open_brackets if x]
    scores = [calculate_score(line) for line in remaining_brackets]
    
    middle = int(len(scores) / 2)
    return sorted(scores)[middle]

incorrect_characters = part_1(DATA)
print(incorrect_characters)

score = part_2(DATA)
print(score)