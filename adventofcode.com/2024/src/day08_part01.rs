use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day08.sample");
    validate::eq(0, solve(sample), "day08 part01 sample");

    let input = include_str!("day08.input");
    validate::eq(0, solve(input), "day08 part01 input");
}