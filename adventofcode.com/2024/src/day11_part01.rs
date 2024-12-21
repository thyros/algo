use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day11.sample");
    validate::eq(0, solve(sample), "day11 part01 sample");

    let input = include_str!("day11.input");
    validate::eq(0, solve(input), "day11 part01 input");
}