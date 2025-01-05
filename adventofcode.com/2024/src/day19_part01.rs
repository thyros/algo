use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day19.sample").trim();
    validate::eq(0, solve(sample), "day19 part01 sample");

    let input = include_str!("day19.input").trim();
    validate::eq(0, solve(input), "day19 part01 input");
}