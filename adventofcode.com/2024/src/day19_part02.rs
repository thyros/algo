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

fn raw_count_design<'a>(design: &'a str, towels: &[&str], cache: &mut HashMap<&'a str, usize>) -> usize {
    // println!("design {}", design);

    if design.is_empty() {
        // println!("\t\t reached end");
        return 1;
    }
    

    towels.iter().filter(|&t| { design.starts_with(t) }).map(|&t| count_design(&design[t.len()..], towels, cache)).sum()
}

fn count_design<'a>(design: &'a str, towels: &[&str], cache: &mut HashMap<&'a str, usize>) -> usize {
    let cached = cache.get(design);
    if cached.is_some() {
        return *cached.unwrap();
    }

    let result = raw_count_design(design, towels, cache);
    cache.insert(design, result);
    return result;    

}

fn solve(input: &str) -> usize {
    let (_, (towels, designs)) = parse(input).unwrap();

    let mut cache: HashMap<&str, usize> = HashMap::default();

    let count = designs.iter().map(|&d| count_design(d, &towels, &mut cache)).sum();

    return count;
}

pub fn main() {
    let sample = include_str!("day19.sample").trim();
    validate::eq(16, solve(sample), "day19 part01 sample");

    let input = include_str!("day19.input").trim();
    validate::eq(251, solve(input), "day19 part01 input");
}
