use super::validate;

use glam::IVec2;
use itertools::Itertools;
use std::collections::{HashMap, HashSet};

type Plot = HashSet<IVec2>;

fn is_same_cell(grid: &Vec<&str>, pos: IVec2, c: char) -> bool {
    let x = pos.x as usize;
    let y = pos.y as usize;

    if y < grid.len() {
        let row = grid[y];

        if x < row.len() {
            return row.chars().nth(x).unwrap() == c;
        }
    }

    return false;
}


fn flood_fill(input: &str) -> Vec<Plot> {
    let lines = input.lines().collect_vec();
    let mut visited: HashSet<IVec2> = HashSet::default();
    let mut result: Vec<Plot> = Vec::default();

    let flood_offsets = [
        IVec2::new(-1, 0),
        IVec2::new(0, -1),
        IVec2::new(1, 0),
        IVec2::new(0, 1),
    ];    

    for (y, line) in lines.iter().enumerate() {
        for (x, c) in line.chars().enumerate() {
            let mut plot = Plot::default();
            let mut open_list: Vec<IVec2> = vec![IVec2::new(x as i32, y as i32)];

            while !open_list.is_empty() {
                let current = open_list.pop().unwrap();
                if visited.contains(&current) {
                    continue;
                }

                plot.insert(current);
                visited.insert(current);
                for o in flood_offsets.iter() {
                    let next = current + o;

                    if is_same_cell(&lines, next, c) {
                        open_list.push(next);
                    }
                }
            }

            if !plot.is_empty() {
                // println!("{}: {:?} {}", c, &plot, evaluate_plot(&plot));
                result.push(plot);
            }
        }
    }

    return result;
}

fn evaluate_plot(plot: &Plot) -> u64 {

    let offsets = [
        IVec2::new(-1, 0),
        IVec2::new(0, -1),
        IVec2::new(1, 0),
        IVec2::new(0, 1),
    ];

    let perimeter: u64 = plot
        .iter()
        .map(|p| {
            offsets
                .iter()
                .map(|o| {
                    if plot.contains(&(p + o)) {
                        return 0;
                    }
                    return 1;
                })
                .sum::<u64>()
        })
        .sum();
        
    let area: u64 = plot.len() as u64;

    return perimeter * area;
}

fn solve(input: &str) -> u64 {
    let result = flood_fill(input)
        .iter()
        // .inspect(|plot| println!("{:?}", plot))
        .map(|plot| evaluate_plot(&plot))
        .sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day12_1.sample").trim();
    validate::eq(80, solve(sample), "day12 part02 sample 1");
    let sample = include_str!("day12_2.sample").trim();
    validate::eq(436, solve(sample), "day12 part01 sample 2");
    let sample = include_str!("day12_4.sample").trim();
    validate::eq(236, solve(sample), "day12 part01 sample 4");
    let sample = include_str!("day12_5.sample").trim();
    validate::eq(236, solve(sample), "day12 part01 sample 5");

    let input = include_str!("day12.input").trim();
    validate::eq(1431440, solve(input), "day12 part01 input");
}
