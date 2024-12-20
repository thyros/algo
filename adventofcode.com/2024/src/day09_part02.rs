use std::collections::HashMap;

use super::validate;

type Range = (i64, i64); // position, length
type Files = HashMap<i64, Range>;
type Blanks = Vec<Range>;

fn solve(input: &str) -> i64 {
    let mut files: Files = Files::default();
    let mut blanks: Blanks = Blanks::default();

    let mut file_id: i64 = 0;
    let mut position: i64 = 0;

    for (i, c) in input.trim().chars().enumerate() {
        let length: i64 = c.to_digit(10).unwrap() as i64;

        if i % 2 == 0 {
            files.insert(file_id, (position, length));
            file_id += 1;
        } else {
            if length != 0 {
                blanks.push((position, length));
            }
        }

        position += length;
    }

    while file_id > 0 {
        file_id -= 1;

        let (pos, size) = files[&file_id];

        for (i, (start, length)) in blanks.iter().enumerate() {
            if start >= &pos {
                blanks.remove(i);
                break;
            }

            if &size <= length {
                files.insert(file_id, (start.clone(), size.clone()));
                if &size == length {
                    blanks.remove(i);
                } else {
                    blanks[i] = (start + size, length - size)
                }
                break;
            }
        }
    }

    let result: i64 = files
        .iter()
        .map(|(file_id, (pos, size))| file_id * size * (pos + pos + size - 1) / 2)
        .sum();

    return result;
}

pub fn main() {
    let sample = include_str!("day09.sample");
    validate::eq(2858, solve(sample), "day09 part02 sample");

    let input = include_str!("day09.input");
    validate::eq(6321896265143, solve(input), "day09 part02 input");
}
