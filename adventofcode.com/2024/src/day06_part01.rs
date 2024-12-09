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

fn solve(input: &str) -> usize {
    let (walls, mut position, board_size) = parse(input);

    let mut visited: HashSet<IVec2> = HashSet::default();
    visited.insert(position);

    let mut result: usize = 1;
    let mut direction = IVec2::NEG_Y;
    let mut guard_on_board: bool = true;

    while guard_on_board {
        let next_position = position + direction;
        if next_position.x >= 0 && next_position.x < board_size.x && next_position.y >= 0 && next_position.y < board_size.y {
            if walls.contains(&next_position) {
                if direction == IVec2::NEG_Y {
                    direction = IVec2::X;
                } else if direction == IVec2::X {
                    direction = IVec2::Y; 
                } else if direction == IVec2::Y {
                    direction = IVec2::NEG_X
                } else {
                    direction = IVec2::NEG_Y;
                }
            } else {
                position = next_position;
                if visited.insert(position) {
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
    validate::eq(41, solve(sample), "day06 part01 sample");

    let input = include_str!("day06.input");
    validate::eq(5080, solve(input), "day06 part01 input");
}