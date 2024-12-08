use std::collections::HashMap;

use super::validate;
use glam::IVec2;

const DIRECTIONS: [[IVec2; 3]; 8] = [
    [IVec2::new(0, 1), IVec2::new(0, 2), IVec2::new(0, 3)],
    [IVec2::new(1, 1), IVec2::new(2, 2), IVec2::new(3, 3)],
    [IVec2::new(1, 0), IVec2::new(2, 0), IVec2::new(3, 0)],
    [IVec2::new(1, -1), IVec2::new(2, -2), IVec2::new(3, -3)],
    [IVec2::new(0, -1), IVec2::new(0, -2), IVec2::new(0, -3)],
    [IVec2::new(-1, -1), IVec2::new(-2, -2), IVec2::new(-3, -3)],
    [IVec2::new(-1, 0), IVec2::new(-2, 0), IVec2::new(-3, 0)],
    [IVec2::new(-1, 1), IVec2::new(-2, 2), IVec2::new(-3, 3)],
];

fn solve(input: &str) -> usize {
    let positions = input
        .lines()
        .enumerate()
        .flat_map(|(y, line)| {
            line.chars().enumerate().map(
                move |(x, value)| {
                    (IVec2::new(x as i32, y as i32), value)
                },
            )
        }).collect::<HashMap<IVec2, char>>();

    let mas = ['M', 'A', 'S'];
    let result: usize = positions
        .iter()
        .filter(|(_position, value)| **value == 'X')
        .map(|(position, _value)| {
            let count = DIRECTIONS
                .iter()
                .map(|offsets| {
                    offsets
                        .iter()
                        .map(|offset| {
                            positions.get(&(position + offset)) })
                        .enumerate()
                        .all(|(index, value)| {
                            mas.get(index) == value})
                })
                .filter(|b| *b)
                .count();
            count
        })
        .sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day04.sample");
    validate::eq(18, solve(sample),"day04 part01 sample");

    let input = include_str!("day04.input");
    validate::eq(2454, solve(input),"day04 part01 input");
}