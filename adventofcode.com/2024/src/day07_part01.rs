use itertools::Itertools;
use nom::{
    bytes::complete::tag,
    character::complete::{self, line_ending, space1},
    multi::separated_list1,
    sequence::separated_pair,
    IResult,
};

use super::validate;

fn parse(input: &str) -> IResult<&str, Vec<(u64, Vec<u64>)>> {
    separated_list1(
        line_ending,
        separated_pair(
            complete::u64,
            tag(": "),
            separated_list1(space1, complete::u64),
        ),
    )(input)
}

fn correct_number(value: &u64, parts: &Vec<u64>) -> bool {
    let max = u64::pow(2, parts.len() as u32 - 1);

    for it in 0..max {
        let mut result: u64 = 0;
        for (i, (a, b)) in parts.iter().tuple_windows().enumerate() {
            if result == 0 {
                result = *a;
            }

            if (it & 1 << i) != 0 {
                result += b
            } else {
                result *= b;
            }
        }

        if &result == value {
            return true;
        }
    }
    return false;
}

fn solve(input: &str) -> u64 {
    let (_input, data) = parse(input).unwrap();

    let correct_numbers = data
        .iter()
        .filter(|(value, parts)| correct_number(value, parts))
        .map(|(value, _)| value).collect_vec();

    return correct_numbers.into_iter().sum();
}

pub fn main() {
    let sample = include_str!("day07.sample");
    validate::eq(3749, solve(sample), "day07 part01 sample");

    let input = include_str!("day07.input");
    validate::eq(7579994664753, solve(input), "day07 part01 input");
}
