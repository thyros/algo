use glam::U64Vec2;
use nom::{
    bytes::complete::tag,
    character::complete::{self, line_ending},
    multi::separated_list1,
    sequence::{preceded, separated_pair, terminated, tuple},
    IResult, Parser,
};

use super::validate;

#[derive(Debug)]
struct Machine {
    a: U64Vec2,
    b: U64Vec2,
    prize: U64Vec2,
}

fn a_button(input: &str) -> IResult<&str, U64Vec2> {
    preceded(
        tag("Button A: X+"),
        separated_pair(complete::u64, tag(", Y+"), complete::u64).map(|(x, y)| U64Vec2::new(x, y)),
    )(input)
}

fn b_button(input: &str) -> IResult<&str, U64Vec2> {
    preceded(
        tag("Button B: X+"),
        separated_pair(complete::u64, tag(", Y+"), complete::u64).map(|(x, y)| U64Vec2::new(x, y)),
    )(input)
}

fn prize(input: &str) -> IResult<&str, U64Vec2> {
    preceded(
        tag("Prize: X="),
        separated_pair(complete::u64, tag(", Y="), complete::u64)
            .map(|(x, y)| U64Vec2::new(10000000000000 + x, 10000000000000 + y)),
    )(input)
}

fn machine(input: &str) -> IResult<&str, Machine> {
    let (input, (a, b, p)) = tuple((
        terminated(a_button, line_ending),
        terminated(b_button, line_ending),
        prize,
    ))(input)?;
    Ok((input, Machine { a, b, prize: p }))
}

fn parse(input: &str) -> Vec<Machine> {
    let result = separated_list1(tuple((line_ending, line_ending)), machine)(input);
    return result.unwrap().1;
}

fn solve(input: &str) -> u64 {
    let machines = parse(input);

    let a_cost: u64 = 3;
    let b_cost: u64 = 1;

    let mut result: u64 = 0;
    for m in machines {
        dbg!(&m);
        let ax: f64 = m.a.x as f64;
        let ay: f64 = m.a.y as f64;
        let bx: f64 = m.b.x as f64;
        let by: f64 = m.b.y as f64;
        let px: f64 = m.prize.x as f64;
        let py: f64 = m.prize.y as f64;
        let ca: f64 = (px * by - py * bx) / (ax * by - ay * bx);
        let cb: f64 = (px - ax * ca) / bx;
        dbg!(ca, cb);
        if ca.floor() == ca && cb.floor() == cb {
            result += ca as u64 * a_cost + cb as u64 * b_cost;
        }
    }

    return result;
}

pub fn main() {
    let sample = include_str!("day13.sample").trim();
    validate::eq(875318608908, solve(sample), "day13 part02 sample");

    let input = include_str!("day13.input").trim();
    validate::eq(95273925552482, solve(input), "day13 part02 input");
}
