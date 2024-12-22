use itertools::Itertools;

use super::validate;

fn parse(input: &str) -> Vec<u64> {
    input.split(' ').map(|c| c.parse::<u64>().unwrap()).collect()
}

fn blink(stones: Vec<u64>) -> Vec<u64> {
    stones.iter().flat_map(|v| {
        if v == &0 {
            return vec![1];
        } 
        let digits = v.ilog10() + 1;
        if digits % 2 == 0 {
            let half = digits / 2;
            let div = 10_u64.pow(half);

            return vec![v / div, v % div];
        }
        
        return vec![v * 2024];
    }).collect_vec()
}

fn solve(input: &str) -> usize {

    let mut stones = parse(input);

    for i in 0..75 {
        // println!("{:?}: {:?}", i, &stones);
        stones = blink(stones);
    }

    return stones.len();
}

pub fn main() {
    let sample = include_str!("day{day}.sample").trim();
    validate::eq(0, solve(sample), "day{day} part{part} sample");

    let input = include_str!("day{day}.input").trim();
    validate::eq(0, solve(input), "day{day} part{part} input");
}