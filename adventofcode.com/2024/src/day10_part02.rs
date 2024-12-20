use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day10.sample");
    validate::eq(0, solve(sample), "day10 part02 sample");

    let input = include_str!("day10.input");
    validate::eq(0, solve(input), "day10 part02 input");
}