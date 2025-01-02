use std::{collections::HashSet, i32};

use glam::IVec2;
use itertools::Itertools;
use nom::{
    bytes::complete::tag,
    character::complete::{self, line_ending},
    multi::separated_list1,
    sequence::{preceded, separated_pair},
    IResult,
};

use super::validate;

#[derive(Debug)]
struct Robot {
    p: IVec2,
    v: IVec2,
}

fn parse_robot(input: &str) -> IResult<&str, Robot> {
    let (input, ((px, py), (vx, vy))) = separated_pair(
        preceded(
            tag("p="),
            separated_pair(complete::i32, tag(","), complete::i32),
        ),
        tag(" "),
        preceded(
            tag("v="),
            separated_pair(complete::i32, tag(","), complete::i32),
        ),
    )(input)?;

    let p = IVec2::new(px, py);
    let v = IVec2::new(vx, vy);

    Ok((input, Robot { p, v }))
}

fn parse(input: &str) -> Vec<Robot> {
    let result = separated_list1(line_ending, parse_robot)(input);
    return result.unwrap().1;
}

fn simulate(robots: &Vec<Robot>, board_size: &IVec2, steps: i32) -> Vec<IVec2> {
    let final_positions = robots
        .iter()
        .map(|r| {
            let mut new_p = r.p + r.v * steps;
            new_p %= board_size;
            if new_p.x < 0 {
                new_p.x += board_size.x;
            }
            if new_p.y < 0 {
                new_p.y += board_size.y;
            }
            return new_p;
        })
        .collect_vec();
    return final_positions;
}

fn tree_test(pos: &Vec<IVec2>) -> bool {
    // I'm assuming the location of each robot must be unique to form a christmas tree
    let mut tree: HashSet<IVec2> = HashSet::default();

    for p in pos {
        if tree.insert(p.clone()) == false {
            return false;
        }
    }

    return true;
}

fn solve(input: &str, board_size: IVec2) -> i32 {
    let robots = parse(input);
    for i in 0..board_size.x * board_size.y {
        let positions = simulate(&robots, &board_size, i);
        if tree_test(&positions) {
            return i
        }
    }
    return 0;
}

pub fn main() {
    let input = include_str!("day14.input").trim();
    validate::eq(6771, solve(input, IVec2::new(101, 103)), "day14 part02 input");
}