use super::validate;
use std::collections::HashMap;

fn solve(input: &str) -> usize {
    let mut left = vec![];
    let mut right: HashMap<usize, usize> = HashMap::new();

    for line in input.lines() {
        let mut items = line.split_whitespace();
        left.push(
            items.next().unwrap().parse::<usize>().unwrap()
        );
        right.entry(items.next().unwrap().parse::<usize>().unwrap())
            .and_modify(|v| {
                *v += 1;
            })
            .or_insert(1);
    }

    let result: usize = 
        left.iter()
            .map(|number| {
                number * right.get(number).unwrap_or(&0)
            })
            .sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day01.sample");
    validate::eq(31, solve(sample),"day01 part02 sample");

    let input = include_str!("day01.input");
    validate::eq(23126924, solve(input),"day01 part02 input");
}