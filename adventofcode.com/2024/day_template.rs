use super::validate;

fn solve1(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

fn solve2(_input: &str) -> usize {
    let result: usize = 0;
    return result;
}

pub fn solve() {
    let sample = include_str!("day{day}.sample");
    validate::eq(0, solve1(sample));
    validate::eq(0, solve2(sample));

    let input = include_str!("day{day}.input");
    validate::eq(0, solve1(input));
    validate::eq(0, solve2(input));
}