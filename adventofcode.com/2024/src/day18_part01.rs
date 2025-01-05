use std::collections::HashSet;

use glam::IVec2;
use nom::{
    bytes::complete::tag,
    character::complete::{self, line_ending},
    multi::separated_list1,
    sequence::separated_pair,
    IResult, Parser,
};
use pathfinding::prelude::dijkstra;

use super::validate;

fn parse(input: &str) -> IResult<&str, Vec<IVec2>> {
    separated_list1(
        line_ending,
        separated_pair(complete::i32, tag(","), complete::i32).map(|(x, y)| IVec2::new(x, y)),
    )(input)
}

fn print_grid(size: i32, path: &HashSet<IVec2>, walls: &HashSet<IVec2>) {
    for y in 0..size {
        for x in 0..size {
            let p = IVec2::new(x, y);
            if walls.contains(&p) {
                print!("#");
            } else if path.contains(&p) {
                print!("O");
            } else {
                print!(".");
            }
        }
        println!("");
    }
}

fn solve(input: &str, size: i32, limit: i32) -> usize {
    let bytes = parse(input).unwrap().1;

    let walls = bytes.into_iter().take(limit as usize).collect::<HashSet<_>>();

    print_grid(size, &HashSet::default(), &walls);

    let result = dijkstra(
        &IVec2::new(0, 0),
        |&p| {
            vec![
                IVec2::new(p.x + 1, p.y),
                IVec2::new(p.x, p.y - 1),
                IVec2::new(p.x - 1, p.y),
                IVec2::new(p.x, p.y + 1),
            ]
            .into_iter()
            .filter(|p| p.x >= 0 && p.y >=0 && p.x < size && p.y < size && !walls.contains(p))
            .map(|p| (p, 1))
        },
        |&p| p == size - IVec2::ONE,
    ).unwrap();

    let path = result.0.into_iter().collect::<HashSet<_>>();

    print_grid(size, &path, &walls);

    return path.len() - 1;  // path contains nodes but as a result I need to count steps
}

pub fn main() {
    let sample = include_str!("day18.sample").trim();
    validate::eq(22, solve(sample, 7, 12), "day18 part01 sample");

    let input = include_str!("day18.input").trim();
    validate::eq(0, solve(input, 71, 1024), "day18 part01 input");
}
