use std::collections::HashMap;

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

fn _print_robots(pos: &Vec<IVec2>, board_size: &IVec2) {
    let mut counts: HashMap<IVec2, i32> = HashMap::default();
    for &p in pos.iter() {
        match counts.get(&p) {
            Some(count) => {
                counts.insert(p, count + 1);
            }
            None => {
                counts.insert(p, 1);
            }
        }
    }

    for y in 0..board_size.y {
        for x in 0..board_size.x {
            let l = counts.get(&IVec2::new(x, y));
            if l.is_some() {
                print!("{}", l.unwrap());
            } else {
                print!(".");
            }
        }
        println!("");
    }
}

fn solve(input: &str, board_size: IVec2, steps: i32) -> i32 {
    let robots = parse(input);
    let positions = simulate(&robots, &board_size, steps);

    // println!("{:?}", positions);
    // print_robots(&positions, &board_size);

    let qs = board_size / 2; // quadrant size
    let (tl, tr, bl, br) = positions
        .iter()
        .fold((0, 0, 0, 0), |(tl, tr, bl, br), pos| {
            if pos.x < qs.x && pos.y < qs.y {
                return (tl + 1, tr, bl, br);
            } else if pos.x > qs.x && pos.y < qs.y {
                return (tl, tr + 1, bl, br);
            } else if pos.x < qs.x && pos.y > qs.y {
                return (tl, tr, bl + 1, br);
            } else if pos.x > qs.x && pos.y > qs.y {
                return (tl, tr, bl, br + 1);
            } else {
                return (tl, tr, bl, br);
            }
        });

    return tl * tr * bl * br;
}

pub fn main() {

    // validate::eq(0, solve("p=2,4 v=2,-3", IVec2::new(11, 7), 5), "day14 part01 sample");

    let sample = include_str!("day14.sample").trim();
    validate::eq(12, solve(sample, IVec2::new(11, 7), 100), "day14 part01 sample");

    let input = include_str!("day14.input").trim();
    validate::eq(231221760, solve(input, IVec2::new(101, 103), 100), "day14 part01 input");
}
