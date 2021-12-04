SAMPLE = [
    "7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1",
    "",
    "22 13 17 11  0",
    " 8  2 23  4 24",
    "21  9 14 16  7",
    " 6 10  3 18  5",
    " 1 12 20 15 19",
    "",
    " 3 15  0  2 22",
    " 9 18 13 17  5",
    "19  8  7 25 23",
    "20 11 10 24  4",
    "14 21 16 12  6",
    "",
    "14 21 17 24  4",
    "10 16 15  9 19",
    "18  8 23 26 20",
    "22 11 13  6  5",
    " 2  0 12  3  7"]


def parse_board(data):
    board = []
    for line in data:
        for number in line.split():
            board.append((int(number), False))
    return board


def parse_boards(data):
    boards = []
    chunk_size = 6
    boards_count = int(len(data) / (chunk_size))

    for y in range(boards_count):
        board_starts = y * chunk_size + 1
        board_ends = y * chunk_size + chunk_size
        boards.append(parse_board(data[board_starts: board_ends]))
    return boards


with open("day-04.input", "r") as fin:
    DATA = fin.readlines()
    DATA = [d.strip() for d in DATA]

def is_winner(board):
    indices = [[0, 1, 2, 3, 4],         # row
               [5, 6, 7, 8, 9],         # row
               [10, 11, 12, 13, 14],    # row
               [15, 16, 17, 18, 19],    # row
               [20, 21, 22, 23, 24],    # row
               [0, 5, 10, 15, 20],      # column
               [1, 6, 11, 16, 21],      # column
               [2, 7, 12, 17, 22],      # column
               [3, 8, 13, 18, 23],      # column
               [4, 9, 14, 19, 24]]      # column

    return any(all(board[i][1] == True for i in line) for line in indices)

def mark(number, board):
    return [(n, True if n == number else v) for n, v in board]

def sum_unmarked_numbers(board):
    return sum(n for n, v in board if v == False)

def part_1(numbers, boards):
    for number in numbers:
        new_boards = []
        for board in boards:
            board = mark(number, board)
            if is_winner(board):
                return sum_unmarked_numbers(board), number
            new_boards.append(board)
        boards = new_boards

    return None

def part_2(numbers, boards):
    for number in numbers:
        new_boards = []
        for board in boards:
            board = mark(number, board)
            if not is_winner(board):
                new_boards.append(board)
            elif len(boards) == 1 and is_winner(board):
                return sum_unmarked_numbers(board), number
        
        boards = new_boards

    return None

def parse(data):
    numbers = [int(x) for x in data[0].split(',')]
    boards = parse_boards(data[1:])
    return numbers, boards

numbers, boards = parse(DATA)

unmarked_numbers, winning_number = part_1(numbers, boards)
print(f"{unmarked_numbers} {winning_number} {unmarked_numbers * winning_number}")

unmarked_numbers, winning_number = part_2(numbers, boards)
print(f"{unmarked_numbers} {winning_number} {unmarked_numbers * winning_number}")