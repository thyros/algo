use std::collections::HashMap;

use nom::{
    bytes::complete::tag,
    character::complete::{alpha1, line_ending, multispace1},
    multi::separated_list1,
    sequence::separated_pair,
    IResult,
};

use super::validate;

fn parse(input: &str) -> IResult<&str, (Vec<&str>, Vec<&str>)> {
    separated_pair(
        separated_list1(tag(", "), alpha1),
        multispace1,
        separated_list1(line_ending, alpha1),
    )(input)
}

fn raw_validate_design<'a>(design: &'a str, towels: &[&str], cache: &mut HashMap<&'a str, bool>) -> bool {
    // println!("design {}", design);

    if design.is_empty() {
        // println!("\t\t reached end");
        return true;
    }
    
    for towel in towels {
        // println!("\t{}", towel);
        if design.starts_with(towel) && validate_design(&design[towel.len()..], towels, cache) {
            return true;
        }
    }

    // println!("\t\tnot possible");
    return false;
}

fn validate_design<'a>(design: &'a str, towels: &[&str], cache: &mut HashMap<&'a str, bool>) -> bool {
    let cached = cache.get(design);
    if cached.is_some() {
        return *cached.unwrap();
    }

    let result = raw_validate_design(design, towels, cache);
    cache.insert(design, result);
    return result;    

}

fn solve(input: &str) -> usize {
    let (_, (towels, designs)) = parse(input).unwrap();

    let mut cache: HashMap<&str, bool> = HashMap::default();

    let count = designs.iter().filter(|design| validate_design(design, &towels, &mut cache)).count();

    return count;
}

pub fn main() {
    let sample = include_str!("day19.sample").trim();
    validate::eq(6, solve(sample), "day19 part01 sample");

    let input = include_str!("day19.input").trim();
    validate::eq(251, solve(input), "day19 part01 input");
}
