use super::validate;

fn solve(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day{day}.sample");
    validate::eq(0, solve(sample));

    let input = include_str!("day{day}.input");
    validate::eq(0, solve(input));
}