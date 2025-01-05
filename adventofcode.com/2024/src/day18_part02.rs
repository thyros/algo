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


fn has_path(walls: &HashSet<&IVec2>, size: i32) -> bool {
    dijkstra(
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
    ).is_some()
}


fn solve(input: &str, size: i32, limit: i32) -> String {
    let bytes = parse(input).unwrap().1;

    let mut l = limit;
    let mut r = bytes.len() as i32 - 1;

    while l < r {
        let m = (r + l) / 2;
        let walls = bytes.iter().take(m as usize + 1).collect::<HashSet<_>>();
        let solvable = has_path(&walls, size);

        if solvable {
            l = m + 1
        } else {
            r = m
        }
    }

    let coord = bytes[l as usize];
    return coord.x.to_string() + "," + &coord.y.to_string();
}

pub fn main() {
    let sample = include_str!("day18.sample").trim();
    validate::eq("6,1".to_string(), solve(sample, 7, 12), "day18 part02 sample");

    let input = include_str!("day18.input").trim();
    validate::eq("61,50".to_string(), solve(input, 71, 1024), "day18 part02 input");
}
