use super::validate;

use itertools::Itertools;
use std::collections::HashMap;
use nom::{
    bytes::complete::tag, character::complete::{self, line_ending}, multi::{fold_many1, separated_list1}, sequence::{separated_pair, terminated}, IResult
};

fn rules(input: &str) -> IResult<&str, HashMap<u32, Vec<u32>>> {
    fold_many1(
        terminated(
            separated_pair(
                complete::u32, 
                tag("|"), 
                complete::u32),
            line_ending),
        HashMap::default,
        |mut acc: HashMap<u32, Vec<u32>>, (page, after)| {
            acc.entry(page).and_modify(|afters| {
                afters.push(after)
            }).or_insert(vec![after]);
            acc
        },
    )(input)
}

fn updates(input: &str) -> IResult<&str, Vec<Vec<u32>>> {
    separated_list1(line_ending, 
        separated_list1(tag(","), complete::u32),
    )(input)
}

fn parse(input: &str) -> IResult<&str, (HashMap<u32, Vec<u32>>, Vec<Vec<u32>>)> {
    let (input, parsed_rules) = terminated(rules, line_ending)(input)?;
    let (input, parsed_updates) = updates(input)?;
    Ok((input, (parsed_rules, parsed_updates)))
}

fn solve(input: &str) -> usize {
    let (_input, (parsed_rules, parsed_updates)) = parse(input).unwrap();

    dbg!(&parsed_rules);
    dbg!(&parsed_updates);

    let results: Vec<usize> = updates
        .iter()
        .enumerate()
        .filter_map(|(index, update)| {

        })
        .collect();

    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day05.sample");
    validate::eq(0, solve(sample), "day05 part01 sample");

    // let input = include_str!("day05.input");
    // validate::eq(0, solve(input), "day05 part01 input");
}