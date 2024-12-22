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

    for _i in 0..25 {
        // println!("{:?}", i);
        stones = blink(stones);
    }

    return stones.len();
}

pub fn main() {
    let sample = include_str!("day11.sample").trim();
    validate::eq(55312, solve(sample), "day11 part01 sample");

    let input = include_str!("day11.input").trim();
    validate::eq(224529, solve(input), "day11 part01 input");
}