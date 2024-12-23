use super::validate;

use glam::{IVec2};
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

fn count_sides(plot: &Plot) -> u64 {
    let mut edges: HashMap<IVec2, IVec2> = HashMap::default();

    for cell in plot {
        let next_cells = [
            IVec2::new(cell.x - 1, cell.y),
            IVec2::new(cell.x, cell.y - 1),
            IVec2::new(cell.x + 1, cell.y),
            IVec2::new(cell.x, cell.y + 1),
        ];

        for next_cell in next_cells {
            if plot.contains(&next_cell) {
                continue;
            }

            // edges are mutliplied by 10 so I can use IVec2
            // as a key in HashMap
            let edge_x: i32 = (cell.x + next_cell.x) * 10 / 2;
            let edge_y: i32 = (cell.y + next_cell.y) * 10 / 2;

            edges.insert(IVec2::new(edge_x, edge_y), IVec2::new(edge_x - cell.x * 10, edge_y - cell.y * 10));
        }
    }

    let mut seen: HashSet<IVec2> = HashSet::default();
    let mut side_count: u64 = 0;

    for (edge, direction) in edges.iter() {
        if seen.contains(edge) {
            continue;
        }

        side_count += 1;

        let ex = edge.x;
        let ey = edge.y;

        if ex % 10 == 0 {
            for dx in vec!(-1, 1) {
                let mut cx = ex + dx;
                while edges.get(&IVec2::new(cx, ey)) == Some(direction) {
                    seen.insert(IVec2::new(cx, ey));
                    cx += dx;
                }
            }
        } else {
            for dy in vec!(-1, 1) {
                let mut cy = ey + dy;
                while edges.get(&IVec2::new(ex, cy)) == Some(direction) {
                    seen.insert(IVec2::new(ex, cy));
                    cy += dy;
                }
            }
        }
    }

    return side_count;
}

fn solve(input: &str) -> u64 {
    let result = flood_fill(input)
        .iter()
        // .inspect(|plot| println!("{:?}", plot))
        .map(|plot| plot.len() as u64 * count_sides(&plot))
        .sum();
    return result;
}

pub fn main() {
    let sample = include_str!("day12_1.sample").trim();
    validate::eq(140, solve(sample), "day12 part01 sample 1");
    let sample = include_str!("day12_2.sample").trim();
    validate::eq(772, solve(sample), "day12 part01 sample 2");
    let sample = include_str!("day12_3.sample").trim();
    validate::eq(1930, solve(sample), "day12 part01 sample 3");

    let input = include_str!("day12.input").trim();
    validate::eq(1431440, solve(input), "day12 part01 input");
}
