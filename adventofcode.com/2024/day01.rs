use std::collections::HashMap;

fn solve1(input: &str) -> i32 {
    let mut left = vec![];
    let mut right = vec![];

    for line in input.lines() {
        let mut items = line.split_whitespace();
        left.push(
            items.next().unwrap().parse::<i32>().unwrap()
        );
        right.push(
            items.next().unwrap().parse::<i32>().unwrap()
        );    
    }

    left.sort();
    right.sort();

    let result: i32 = std::iter::zip(left, right)
        .map(|(l, r)| (l - r).abs())
        .sum();

    return result;
}

fn solve2(input: &str) -> usize {
    let mut left = vec![];
    let mut right: HashMap<usize, usize> = HashMap::new();

    for line in input.lines() {
        let mut items = line.split_whitespace();
        left.push(
            items.next().unwrap().parse::<usize>().unwrap()
        );
        right.entry(items.next().unwrap().parse::<usize>().unwrap())
            .and_modify(|v| {
                *v += 1;
            })
            .or_insert(1);
    }

    let result: usize = 
        left.iter()
            .map(|number| {
                number * right.get(number).unwrap_or(&0)
            })
            .sum();
    return result;
}

fn main() {
    let file = include_str!("day01.input");

    println!("Result part 1: {}", solve1(file));
    println!("Result part 2: {}", solve2(file));
}