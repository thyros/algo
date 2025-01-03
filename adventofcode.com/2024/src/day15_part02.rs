use std::collections::{HashMap, HashSet};

use glam::IVec2;
use itertools::Itertools;

use super::validate;
#[derive(Debug)]
struct Warehouse {
    grid: Vec<Vec<char>>,
    robot: IVec2,
}

fn parse_warehouse(input: &str) -> Warehouse {
    let mut grid: Vec<Vec<char>> = Vec::default();
    let mut robot: IVec2 = IVec2::ZERO;

    for (y, line) in input.lines().enumerate() {
        let mut row: Vec<char> = Vec::default();

        for (x, c) in line.chars().enumerate() {
            match c {
                '#' => {
                    row.extend(['#', '#']);
                }
                'O' => {
                    row.extend(['[', ']']);
                }
                '@' => {
                    row.extend(['.', '.']);
                    robot = IVec2::new((x * 2) as i32, y as i32);
                }
                _ => {
                    row.extend(['.', '.']);
                }
            }
        }

        grid.push(row);
    }

    return Warehouse { grid, robot };
}

fn parse_commands(input: &str) -> String {
    input.replace("\r\n", "")
}

fn _print_warehouse(warehouse: &Warehouse) {
    for (y, row) in warehouse.grid.iter().enumerate() {
        for (x, c) in row.iter().enumerate() {
            if warehouse.robot.x == x as i32 && warehouse.robot.y == y as i32 {
                print!("@");
            } else {
                print!("{}", c);
            }
        }
        println!();
    }
}

fn push_horizontal(warehouse: &mut Warehouse, dir: IVec2) {
    let a_pos = warehouse.robot + dir;

    if warehouse.grid[a_pos.y as usize][a_pos.x as usize] == '.' {
        warehouse.robot = a_pos;
        return;
    }

    let mut b_pos = a_pos;
    loop {
        let obj = warehouse.grid[b_pos.y as usize][b_pos.x as usize];
        match obj {
            '#' => {
                break;
            }
            '[' => {
                b_pos += dir;
            }
            ']' => {
                b_pos += dir;
            }
            '.' => {
                while b_pos.x != a_pos.x {
                    warehouse.grid[b_pos.y as usize][b_pos.x as usize] =
                        warehouse.grid[b_pos.y as usize][(b_pos.x - dir.x) as usize];
                    b_pos.x -= dir.x;
                }

                warehouse.grid[a_pos.y as usize][a_pos.x as usize] = '.';
                warehouse.robot = a_pos;
                break;
            }
            _ => {
                panic!("Unrecognized command")
            }
        }
    }
}

fn can_push(
    warehouse: &Warehouse,
    pos: IVec2,
    dir: IVec2,
    checked: &mut HashMap<IVec2, bool>,
) -> bool {
    let c = warehouse.grid[pos.y as usize][pos.x as usize];
    if c == '.' {
        return true;
    } else if c == '#' {
        return false;
    }

    if checked.contains_key(&pos) {
        return *checked.get(&pos).unwrap();
    }

    let a_pos = pos + dir;
    if c == '[' {
        let can_push = can_push(warehouse, a_pos, dir, checked)
            && can_push(warehouse, a_pos + IVec2::new(1, 0), dir, checked);
        checked.insert(pos, can_push);
        return can_push;
    } else if c == ']' {
        let can_push = can_push(warehouse, a_pos, dir, checked)
            && can_push(warehouse, a_pos + IVec2::new(-1, 0), dir, checked);
        checked.insert(pos, can_push);
        return can_push;
    }
    return false;
}

fn push(warehouse: &mut Warehouse, pos: IVec2, dir: IVec2) {
    // I'm probably pushing the same boxes more then once

    let c = warehouse.grid[pos.y as usize][pos.x as usize];
    let a_pos = pos + dir;
    if c == '[' {
        push(warehouse, a_pos, dir);
        push(warehouse, a_pos + IVec2::new(1, 0), dir);
        warehouse.grid[a_pos.y as usize][a_pos.x as usize] =
            warehouse.grid[pos.y as usize][pos.x as usize];
        warehouse.grid[a_pos.y as usize][(a_pos.x + 1) as usize] =
            warehouse.grid[pos.y as usize][(pos.x + 1) as usize];
        warehouse.grid[pos.y as usize][pos.x as usize] = '.';
        warehouse.grid[pos.y as usize][(pos.x + 1) as usize] = '.';
    } else if c == ']' {
        push(warehouse, a_pos, dir);
        push(warehouse, a_pos + IVec2::new(-1, 0), dir);
        warehouse.grid[a_pos.y as usize][a_pos.x as usize] =
            warehouse.grid[pos.y as usize][pos.x as usize];
        warehouse.grid[a_pos.y as usize][(a_pos.x - 1) as usize] =
            warehouse.grid[pos.y as usize][(pos.x - 1) as usize];
        warehouse.grid[pos.y as usize][pos.x as usize] = '.';
        warehouse.grid[pos.y as usize][(pos.x - 1) as usize] = '.';
    }
}

fn push_vertially(warehouse: &mut Warehouse, dir: IVec2) {
    let mut checked: HashMap<IVec2, bool> = HashMap::default();
    let a_pos = warehouse.robot + dir;
    if can_push(warehouse, a_pos, dir, &mut checked) {
        push(warehouse, a_pos, dir);
        warehouse.robot = a_pos;
    }
}

fn simulate(mut warehouse: Warehouse, commands: &str) -> HashSet<IVec2> {
    // print_warehouse(&warehouse);
    for c in commands.chars() {
        // println!("Command: {}", c);
        match c {
            '<' => {
                push_horizontal(&mut warehouse, IVec2::new(-1, 0));
            }
            '>' => {
                push_horizontal(&mut warehouse, IVec2::new(1, 0));
            }
            '^' => {
                push_vertially(&mut warehouse, IVec2::new(0, -1));
            }
            'v' => {
                push_vertially(&mut warehouse, IVec2::new(0, 1));
            }
            _ => {
                panic!("Unrecongnized command");
            }
        }
        // print_warehouse(&warehouse);
    }

    let mut boxes: HashSet<IVec2> = HashSet::default();
    for (y, row) in warehouse.grid.iter().enumerate() {
        for (x, c) in row.iter().enumerate() {
            if c == &'[' {
                boxes.insert(IVec2::new(x as i32, y as i32));
            }
        }
    }
    return boxes;
}

fn parse(input: &str) -> (Warehouse, String) {
    let parts = input.split("\r\n\r\n").collect_vec();
    let warehouse = parse_warehouse(parts[0]);
    let commands = parse_commands(parts[1]);

    return (warehouse, commands);
}

fn count_result(boxes: HashSet<IVec2>) -> i32 {
    boxes
        .iter()
        .map(|b| {
            return 100 * b.y + b.x;
        })
        .sum()
}

fn solve(input: &str) -> i32 {
    let (warehouse, commands) = parse(input);
    // println!("{:?}\n{:?}", &warehouse, &commands);

    let boxes = simulate(warehouse, &commands);

    let result = count_result(boxes);
    return result;
}

pub fn main() {
    let sample = include_str!("day15_2.sample").trim();
    validate::eq(9021, solve(sample), "day15 part02 sample");

    let input = include_str!("day15.input").trim();
    validate::eq(1521952, solve(input), "day15 part02 input");
}
