use super::validate;

fn solve(input: &str) -> i32 {
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

pub fn main() {
    let sample = include_str!("day01.sample");
    validate::eq(11, solve(sample),"day01 part01 sample");

    let input = include_str!("day01.input");
    validate::eq(1506483, solve(input),"day01 part01 input");
}