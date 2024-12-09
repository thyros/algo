use std::{cmp, collections::HashSet};

use glam::IVec2;

use super::validate;

fn parse(input: &str) -> (Vec<IVec2>, IVec2, IVec2) {
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

fn guard_loops(board_size: IVec2, walls: Vec<IVec2>, visited: &HashSet<IVec2>, start: IVec2, dir: IVec2) -> bool {
    let mut position = start;
    let mut direction = dir;

    loop {
        let next_position = position + direction;      
        if next_position == start {
            return true;
        } else if !is_on_board(next_position, board_size) {
            return false;
        } if walls.contains(&next_position) {
            direction = rotate(direction);
        } else if !visited.contains(&next_position) {
            return false;
        } else {
            position = next_position;
        }
    }
}

fn solve(input: &str) -> usize {
    let (walls, mut position, board_size) = parse(input);

    let mut visited: HashSet<IVec2> = HashSet::default();
    visited.insert(position);

    let mut result: usize = 0;
    let mut direction = IVec2::NEG_Y;
    let mut guard_on_board: bool = true;

    while guard_on_board {
        let next_position = position + direction;
        if is_on_board(next_position, board_size) {
            if walls.contains(&next_position) {
                direction = rotate(direction);
            } else {
                position = next_position;
                visited.insert(position);
                
                let mut new_walls: Vec<IVec2> = walls.clone();
                new_walls.push(position + direction);
                
                if guard_loops(board_size, new_walls, &visited, position, direction) {
                    result += 1;
                }
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