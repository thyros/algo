use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day21.sample").trim();
    validate::eq(126384, solve(sample), "day21 part01 sample");

    let input = include_str!("day21.input").trim();
    validate::eq(0, solve(input), "day21 part01 input");
}