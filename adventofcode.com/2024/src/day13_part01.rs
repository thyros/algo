use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day13.sample").trim();
    validate::eq(0, solve(sample), "day13 part01 sample");

    let input = include_str!("day13.input").trim();
    validate::eq(0, solve(input), "day13 part01 input");
}