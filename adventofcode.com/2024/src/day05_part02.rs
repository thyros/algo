use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day05.sample");
    validate::eq(0, solve(sample), "day05 part02 sample");

    let input = include_str!("day05.input");
    validate::eq(0, solve(input), "day05 part02 input");
}