use super::validate;

use glam::IVec2;
use itertools::Itertools;
use std::collections::{HashMap, HashSet};

type Cursor = (IVec2, u32); // (pos, height)
type Map = HashMap<IVec2, u32>;

pub fn parse(input: &str) -> (Map, IVec2) {
    let lines = input.lines().collect_vec();
    let size: IVec2 = IVec2::new(lines[0].len() as i32, lines.len() as i32);

    let mut result: Map = HashMap::default();
    for (y, line) in lines.iter().enumerate() {
        for (x, c) in line.chars().enumerate() {
            result.insert(IVec2::new(x as i32, y as i32), c.to_digit(10).unwrap());
        }
    }

    return (result, size);
}

fn neighbours(map: &Map, size: IVec2, pos: &Cursor) -> HashSet<Cursor> {
    let x_bound = 0..size.x as i32;
    let y_bound = 0..size.y as i32;

    let offsets = [
        IVec2::new(-1, 0),
        IVec2::new(0, -1),
        IVec2::new(1, 0),
        IVec2::new(0, 1),
    ];

    return offsets
        .iter()
        .filter(|&&offset| {
            let p: IVec2 = pos.0 + offset;
            return x_bound.contains(&p.x) && y_bound.contains(&p.y) && map[&p] == pos.1 + 1;
        })
        .map(|offset| (pos.0 + offset, pos.1 + 1))
        .collect::<HashSet<_>>();
}

fn find_peaks(map: &Map, size: IVec2, trailhead: &Cursor) -> HashSet<Cursor> {
    let mut current = neighbours(&map, size, &trailhead);

    let mut step = 0;
    while !current.is_empty() {
        let mut next = HashSet::<Cursor>::default();
        // println!("c: {:?}", current);
        for c in &current {
            let n = neighbours(&map, size, c);
            // println!("\tn: {:?}", n);
            next.extend(n);
        }
        current = next;
        
        step += 1;
        if step == 8 {
            break;
        }
    }
    // println!("return: {:?}", current);
    return current;
}

fn solve(input: &str) -> usize {
    let (map, size) = parse(input);

    let trailheads = map
        .iter()
        .filter(|(_, h)| *h == &0)
        .map(|(&p, &h)| (p, h))
        .collect_vec();

    
    let mut result: usize = 0;
    for trailhead in trailheads {
        // println!("Trailhead {:?}", trailhead);
        let peaks = find_peaks(&map, size, &trailhead);
        // println!("\tpeaks {:?}", peaks);

        result += peaks.len();
    }


    return result;
}

pub fn main() {
    let sample = include_str!("day10.sample").trim();
    validate::eq(36, solve(sample), "day10 part01 sample");

    let input = include_str!("day10.input").trim();
    validate::eq(798, solve(input), "day10 part01 input");
}
