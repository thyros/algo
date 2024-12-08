use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day{day}.sample");
    validate::eq(0, solve(sample), "day{day} part{part} sample");

    let input = include_str!("day{day}.input");
    validate::eq(0, solve(input), "day{day} part{part} input");
}