use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day07.sample");
    validate::eq(0, solve(sample), "day07 part01 sample");

    let input = include_str!("day07.input");
    validate::eq(0, solve(input), "day07 part01 input");
}