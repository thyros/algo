use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day22.sample").trim();
    validate::eq(0, solve(sample), "day22 part02 sample");

    let input = include_str!("day22.input").trim();
    validate::eq(0, solve(input), "day22 part02 input");
}