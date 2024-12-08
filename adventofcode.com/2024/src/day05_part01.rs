use super::validate;

use itertools::Itertools;
use nom::{
    bytes::complete::tag, character::complete::{self, line_ending}, multi::separated_list1, IResult
};

fn rules(input: &str) -> IResult<&str, Vec<Vec<u32>>> {
    separated_list1(line_ending, 
        separated_list1(tag("|"), complete::u32),
    )(input)
}

fn updates(input: &str) -> IResult<&str, Vec<Vec<u32>>> {
    separated_list1(line_ending, 
        separated_list1(tag(","), complete::u32),
    )(input)
}

fn parse(input: &str) -> IResult<&str, (Vec<Vec<u32>>, Vec<Vec<u32>>)> {
    let (input, parsed_rules) = rules(input)?;
    let (input, parsed_updates) = updates(input.trim_start())?;

    Ok((input, (parsed_rules, parsed_updates)))
}

fn valid_update(rules: &Vec<Vec<u32>>, update: &Vec<u32>) -> bool {
    let invalid = rules
        .iter()
        .any(|rule| {
            let l = rule.get(0).unwrap();
            let r = rule.get(1).unwrap();

            if !update.contains(l) || !update.contains(r) {
                return false;
            }

            let left_index = update.iter().position(|v| v == l).unwrap();
            let right_index = update.iter().position(|v| v == r).unwrap();

            return left_index >= right_index;
        });

    return !invalid;
}

fn solve(input: &str) -> u32 {

    let (_input, (parsed_rules, parsed_updates)) = parse(input).unwrap();

    let valid_updates = parsed_updates
        .iter()
        .filter(|u| valid_update(&parsed_rules, &u))
        .collect_vec();


    let mids = valid_updates
        .iter()
        .map(|u| u.get(u.len() / 2).unwrap());

    let result: u32 = mids.sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day05.sample");
    validate::eq(143, solve(sample), "day05 part01 sample");

    let input = include_str!("day05.input");
    validate::eq(5268, solve(input), "day05 part01 input");
}