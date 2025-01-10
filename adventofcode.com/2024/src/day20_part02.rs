use super::validate;
use glam::IVec2;
use itertools::Itertools;
use std::{cmp, collections::HashSet};

type Maze = HashSet<IVec2>;

fn parse_maze(input: &str) -> (Maze, IVec2, IVec2, IVec2) {
    let mut maze: Maze = Maze::default();
    let mut start: IVec2 = IVec2::ZERO;
    let mut end: IVec2 = IVec2::ZERO;
    let mut size: IVec2 = IVec2::ZERO;

    for (y, line) in input.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            let p = IVec2::new(x as i32, y as i32);
            match c {
                'S' => {
                    start = p;
                }
                'E' => {
                    end = p;
                }
                '#' => {
                    maze.insert(p);
                }
                _ => {}
            }
            size.x = cmp::max(size.x, x as i32 + 1);
        }
        size.y = cmp::max(size.y, y as i32 + 1);
    }

    return (maze, start, end, size);
}

fn find_path(walls: &HashSet<IVec2>, start: IVec2, end: IVec2) -> Vec<IVec2> {
    let mut seen: HashSet<IVec2> = HashSet::default();
    let mut path: Vec<IVec2> = Vec::default();

    let mut c = start;

    while c != end {
        path.push(c);
        seen.insert(c);

        let np = vec![
            IVec2::new(c.x + 1, c.y),
            IVec2::new(c.x, c.y - 1),
            IVec2::new(c.x - 1, c.y),
            IVec2::new(c.x, c.y + 1),
        ]
        .into_iter()
        .filter(|p| !walls.contains(p) && !seen.contains(p))
        .take(1)
        .collect_vec()[0];

        c = np;
    }

    return path;
}

fn solve(input: &str, min_save: usize) -> usize {
    let (walls, start, end, _size) = parse_maze(input);

    let path = find_path(&walls, start, end);
    // print_grid(&size, &path, &walls);



    let cheat_length = 20;

    let result = path.iter().enumerate().tuple_combinations().filter_map(
        |((start_cost, start_pos), (end_cost, end_pos))| {
            let distance = (start_pos - end_pos).abs().element_sum() as usize;
            if distance > cheat_length {
                return None;
            }
            let savings = end_cost - start_cost - distance;
            Some(savings)
        }).filter(|&savings| savings >= min_save).count();

    return result;
}

pub fn main() {
    let sample = include_str!("day20.sample").trim();
    validate::eq(285, solve(sample, 50), "day20 part01 sample");

    let input = include_str!("day20.input").trim();
    validate::eq(0, solve(input, 100), "day20 part01 input");
}
