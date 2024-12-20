use itertools::Itertools;

use super::validate;

fn solve(input: &str) -> i64 {
    let mut file_id: i64 = 0i64;

    let mut disk = input
        .trim()
        .chars()
        .enumerate()
        .flat_map(|(i, c)| {
            let d = c.to_digit(10).unwrap() as usize;

            if (i % 2) == 0 {
                let id = file_id;
                file_id += 1;
                return vec![id; d];
            }
            return vec![-1; d];
        })
        .collect_vec();

    let mut l: usize = 0;
    let mut r: usize = disk.len() - 1;
    while l <= r {
        if disk[l] >= 0 {
            l += 1;
        } else if disk[r] == -1 {
            r -= 1;
        } else {
            disk.swap(l, r);
        }
    }

    let result = disk
        .iter()
        .enumerate()
        .filter(|(_, v)| v.is_positive())
        .map(|(i, v)| i as i64 * v)
        .sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day09.sample");
    validate::eq(1928, solve(sample), "day09 part01 sample");

    let input = include_str!("day09.input");
    validate::eq(6288599492129, solve(input), "day09 part01 input");
}
