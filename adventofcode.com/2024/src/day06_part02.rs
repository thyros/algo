use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day06.sample");
    validate::eq(0, solve(sample), "day06 part02 sample");

    let input = include_str!("day06.input");
    validate::eq(0, solve(input), "day06 part02 input");
}