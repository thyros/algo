use super::validate;
use glam::IVec2;
use itertools::Itertools;
use std::{
    cmp,
    collections::{HashMap, HashSet},
};

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

fn print_grid(size: &IVec2, path: &HashMap<IVec2, i32>, walls: &HashSet<IVec2>) {
    for y in 0..size.y {
        for x in 0..size.x {
            let p = IVec2::new(x, y);
            if walls.contains(&p) {
                print!(" #");
            } else if path.get(&p).is_some() {
                print!("{:>2}", path.get(&p).unwrap());
            } else {
                print!(" .");
            }
        }
        println!("");
    }
}

fn find_cheats(path: &HashMap<IVec2, i32>) -> Vec<i32> {
    let mut cheats: Vec<i32> = Vec::default();
    for (p, c) in path {
        for n in vec![
            IVec2::new(p.x + 2, p.y),
            IVec2::new(p.x, p.y - 2),
            IVec2::new(p.x - 2, p.y),
            IVec2::new(p.x, p.y + 2),
        ] {
            let v = path.get(&n);
            if v.is_some() && v.unwrap() - c > 2 {
                let c2 = v.unwrap();
                let saving = c2 - c - 2;

                println!("Cheat found: {}->{} saving {} ({} vs {})", p, n, saving, c2, c);

                cheats.push(saving);
            }
        }
    }

    cheats.sort();
    return cheats;
}

fn bfs(walls: &HashSet<IVec2>, start: IVec2, end: IVec2) -> HashMap<IVec2, i32> {
    let mut p = start;
    let mut cost: i32 = 0;

    let mut path: HashMap<IVec2, i32> = HashMap::default();
    path.insert(start, 0);

    while p != end {
        let np = vec![
            IVec2::new(p.x + 1, p.y),
            IVec2::new(p.x, p.y - 1),
            IVec2::new(p.x - 1, p.y),
            IVec2::new(p.x, p.y + 1),
        ]
        .into_iter()
        .filter(|p| !walls.contains(p) && !path.contains_key(p))
        .take(1)
        .collect_vec()[0];

        cost += 1;
        p = np;
        path.insert(np, cost);
    }

    return path;
}

fn solve(input: &str, min_save: i32) -> usize {
    let (walls, start, end, _size) = parse_maze(input);

    let path = bfs(&walls, start, end);
    // print_grid(&size, &path, &walls);

    let cheats = find_cheats(&path);
    println!("{:?}", cheats);

    let result = cheats.iter().filter(|&&c| c > min_save).count();
    return result;
}

pub fn main() {
    let sample = include_str!("day20.sample").trim();
    validate::eq(41, solve(sample, 30), "day20 part01 sample");

    let input = include_str!("day20.input").trim();
    validate::eq(0, solve(input, 100), "day20 part01 input");
}
