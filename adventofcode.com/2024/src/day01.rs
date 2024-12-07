use super::validate;
use std::collections::HashMap;

fn solve1(input: &str) -> i32 {
    let mut left = vec![];
    let mut right = vec![];

    for line in input.lines() {
        let mut items = line.split_whitespace();
        left.push(
            items.next().unwrap().parse::<i32>().unwrap()
        );
        right.push(
            items.next().unwrap().parse::<i32>().unwrap()
        );    
    }

    left.sort();
    right.sort();

    let result: i32 = std::iter::zip(left, right)
        .map(|(l, r)| (l - r).abs())
        .sum();

    return result;
}

fn solve2(input: &str) -> usize {
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

pub fn solve() {
    let sample = include_str!("day01.sample");
    validate::eq(11, solve1(sample));
    validate::eq(31, solve2(sample));

    let input = include_str!("day01.input");
    validate::eq(1506483, solve1(input));
    validate::eq(23126924, solve2(input));
}