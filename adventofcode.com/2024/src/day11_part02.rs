use std::collections::HashMap;

use super::validate;

fn parse(input: &str) -> Vec<u64> {
    input.split(' ').map(|c| c.parse::<u64>().unwrap()).collect()
}

fn raw_count(stone: u64, steps_remaining: u64, cache: &mut HashMap<(u64, u64), u64>) -> u64 {
    if steps_remaining == 0 {
        return 1;
    } else if stone == 0 {
        return count(1, steps_remaining - 1, cache);
    }

    let digits = stone.ilog10() + 1;
    if digits % 2 == 0 {
        let half = digits / 2;
        let div = 10_u64.pow(half);

        return count(stone / div, steps_remaining - 1, cache) + count(stone % div, steps_remaining - 1, cache);
    }
    return count(stone * 2024, steps_remaining - 1, cache);
}

fn count(stone: u64, steps_remaining: u64, cache: &mut HashMap<(u64, u64), u64>) -> u64 {
    let cached = cache.get(&(stone, steps_remaining));
    if cached.is_some() {
        return cached.unwrap().clone();
    }

    let result = raw_count(stone, steps_remaining, cache);
    cache.insert((stone, steps_remaining), result);
    return result;
}

fn solve(input: &str) -> u64 {
    let stones = parse(input);

    let mut cache: HashMap<(u64, u64), u64> = HashMap::default();

    stones.iter().map(|s| count(*s, 75, &mut cache)).sum()
}

pub fn main() {
    let sample = include_str!("day11.sample").trim();
    validate::eq(65601038650482, solve(sample), "day11 part02 sample");

    let input = include_str!("day11.input").trim();
    validate::eq(266820198587914, solve(input), "day11 part02 input");
}