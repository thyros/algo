use std::{cmp, collections::HashSet};

use glam::IVec2;

use super::validate;

type Walls = Vec<IVec2>;
type Visited = HashSet<(IVec2, IVec2)>;

fn parse(input: &str) -> (Walls, IVec2, IVec2) {
    let mut walls: Vec<IVec2> = Vec::default();
    let mut position: IVec2 = IVec2::default();
    let mut board_size: IVec2 = IVec2::ZERO;

    let _: Vec<_> = input.lines()
        .enumerate()
        .inspect(|(y, line)| {
            let _: Vec<_> = line.chars().enumerate().inspect(|(x, c)| {
                if c == &'#' {
                    walls.push(IVec2::new(*x as i32, *y as i32));
                } else if c == &'^' {
                    position = IVec2::new(*x as i32, *y as i32);
                }

                board_size.x = cmp::max(board_size.x, *x as i32);
                board_size.y = cmp::max(board_size.y, *y as i32);
            }).collect();
        }).collect();

    return (walls, position, board_size + IVec2::ONE);
}

fn rotate(direction: IVec2) -> IVec2 {
    if direction == IVec2::NEG_Y {
        return IVec2::X;
    } else if direction == IVec2::X {
        return IVec2::Y; 
    } else if direction == IVec2::Y {
        return IVec2::NEG_X
    } 
    return IVec2::NEG_Y;
}

fn is_on_board(position: IVec2, size: IVec2) -> bool {
    position.x >= 0 && position.x < size.x && position.y >= 0 && position.y < size.y
}

fn draw_state(board_size: &IVec2, walls: &Walls, visited: &Visited, position: IVec2) {
    for y in 0..board_size.y {
        let mut line: String = String::default();
        for x in 0..board_size.x {
            let pos = IVec2::new(x, y);
            if pos == position {
                line.push('X');
            } else if walls.contains(&pos) {
                line.push('#');
            } else if visited.contains(&(pos, IVec2::X)){
                line.push('>');
            } else if visited.contains(&(pos, IVec2::NEG_X)){
                line.push('<');
            } else if visited.contains(&(pos, IVec2::Y)){
                line.push('v');
            } else if visited.contains(&(pos, IVec2::NEG_Y)){
                line.push('^');
            } else {
                line.push('.');
            }
        }
        println!("{}", line);
    }
    println!("");
}


fn guard_loops(board_size: IVec2, walls: &Vec<IVec2>, start: IVec2, dir: IVec2) -> bool {
    let mut position = start;
    let mut direction = dir;

    let mut visited: Visited = HashSet::default();
    loop {
        visited.insert((position, direction));

        // draw_state(&board_size, walls, &visited, position);
        let next_position = position + direction;
        if visited.contains(&(next_position, direction)) {
            // println!("\t{} {} start", next_position, direction);
            return true;
        } else if !is_on_board(next_position, board_size) {
            // println!("\t{} {} off board", next_position, direction);
            return false;
        } if walls.contains(&next_position) {
            // println!("\t{} {} wall", next_position, direction);
            direction = rotate(direction);
        } else {
            position = next_position;
        }
    }
}

fn solve(input: &str) -> usize {
    let (mut walls, mut position, board_size) = parse(input);

    let mut result: usize = 0;
    let mut direction = IVec2::NEG_Y;
    let mut guard_on_board: bool = true;

    let mut count: i32 = 0;

    while guard_on_board {
        let next_position = position + direction;
        if is_on_board(next_position, board_size) {
            if walls.contains(&next_position) {
                direction = rotate(direction);
            } else {
                walls.push(next_position);
                let loops = guard_loops(board_size, &walls, position, direction);
                if loops {
                    result += 1;
                }
                walls.pop();

                count += 1;
                println!("guard_loops: {}: {} {} {}", count, position, direction, loops);

                position = next_position;
                
            }
        } else {
            guard_on_board = false;
        }
    }

    return result;
}

pub fn main() {
    let sample = include_str!("day06.sample");
    validate::eq(6, solve(sample), "day06 part02 sample");

    let input = include_str!("day06.input");
    validate::eq(0, solve(input), "day06 part02 input");
}