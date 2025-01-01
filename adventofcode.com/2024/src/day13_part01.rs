use glam::UVec2;
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
    a: UVec2,
    b: UVec2,
    prize: UVec2,
}

fn a_button(input: &str) -> IResult<&str, UVec2> {
    preceded(
        tag("Button A: X+"),
        separated_pair(complete::u32, tag(", Y+"), complete::u32).map(|(x, y)| UVec2::new(x, y)),
    )(input)
}

fn b_button(input: &str) -> IResult<&str, UVec2> {
    preceded(
        tag("Button B: X+"),
        separated_pair(complete::u32, tag(", Y+"), complete::u32).map(|(x, y)| UVec2::new(x, y)),
    )(input)
}

fn prize(input: &str) -> IResult<&str, UVec2> {
    preceded(
        tag("Prize: X="),
        separated_pair(complete::u32, tag(", Y="), complete::u32).map(|(x, y)| UVec2::new(x, y)),
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

fn solve(input: &str) -> u32 {
    let machines = parse(input);

    let a_cost: u32 = 3;
    let b_cost: u32 = 1;

    let mut result: u32 = 0;
    for machine in machines {
        for a in 0..=100 {
            for b in 0..=100 {
                if machine.prize == machine.a * a + machine.b * b {
                    result += a * a_cost + b * b_cost;
                }
            }
        }
    }

    return result;
}

pub fn main() {
    let sample = include_str!("day13.sample").trim();
    validate::eq(480, solve(sample), "day13 part01 sample");

    let input = include_str!("day13.input").trim();
    validate::eq(31552, solve(input), "day13 part01 input");
}
