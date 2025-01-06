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

// to find the path through the maze
fn dfs(walls: &HashSet<IVec2>, start: IVec2, end: IVec2) -> HashMap<IVec2, i32> {
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

// fn flood_fill(point: IVec2, path: &HashMap<IVec2, i32>, limit: i32) -> HashSet<IVec2> {
//     let mut seen: HashSet<IVec2> = HashSet::default();
//     let mut open: Vec<(IVec2, i32)> = Vec::default();
//     let mut shortcuts: HashSet<IVec2> = HashSet::default();

//     open.push((point, 0));

//     while !open.is_empty() {
//         let (wall, c) = open.pop().unwrap();
//         seen.insert(wall);

//         for p in [
//             IVec2::new(wall.x + 1, wall.y),
//             IVec2::new(wall.x, wall.y - 1),
//             IVec2::new(wall.x - 1, wall.y),
//             IVec2::new(wall.x, wall.y + 1),
//         ] {
//             if seen.contains(&p) {
//                 continue;
//             }
//             if path.contains_key(&p) {
//                 let next_c = path.get(&p).unwrap();
//                 let distance = next_c - c;
//                 if next_c - c > 

//                 shortcuts.insert(p);
//                 continue;
//             }
//             if c < limit {
//                 open.push((IVec2::new(p.x + 1, p.y), c + 1));
//                 open.push((IVec2::new(p.x, p.y - 1), c + 1));
//                 open.push((IVec2::new(p.x - 1, p.y), c + 1));
//                 open.push((IVec2::new(p.x, p.y + 1), c + 1));
//             }
//         }
//     }

//     return shortcuts;
// }

// to find all shortcuts through the walls

// fn find_cheats(path: &HashMap<IVec2, i32>, len: i32) {
//     for (point, cost) in path {
//         let walls_around = vec![
//             IVec2::new(point.x + 1, point.y),
//             IVec2::new(point.x, point.y - 1),
//             IVec2::new(point.x - 1, point.y),
//             IVec2::new(point.x, point.y + 1),
//         ]
//         .into_iter()
//         .filter(|np| !path.contains_key(np))
//         .collect_vec();

//         for wall in walls_around {

//             let shortcuts = flood_fill(wall, path, len);
//             dbg!(shortcuts);
//         }
//     } 
// }

fn find_cheats(path: &HashMap<IVec2, i32>, size: IVec2, len: i32) -> HashMap<(IVec2, IVec2), i32> {
    let mut result: HashMap<(IVec2, IVec2), i32> = HashMap::default();

    for (p, c) in path {
        for r in 2..=len {
            for dx in 0..=r {
                let dy = r - dx;

                let possible_shortcuts = vec![
                    IVec2::new(p.x + dx, p.y + dy),
                    IVec2::new(p.x + dx, p.y - dy),
                    IVec2::new(p.x - dx, p.y + dy),
                    IVec2::new(p.x - dx, p.y - dy),
                ];

                for np in possible_shortcuts {
                    if np.x < 0 || np.y < 0 || np.x >= size.x || np.y >= size.y {
                        continue;
                    }

                    let exit = path.get(&np);
                    if exit.is_none() {
                        continue;
                    }

                    let c2 = exit.unwrap();
                    if c2 - c > r {
                        result.insert((*p, np), c2 - c - 2);
                    }
                }
            }
        }
    }

    return result;
}

fn solve(input: &str, min_save: i32) -> usize {
    let (walls, start, end, _size) = parse_maze(input);

    let path = dfs(&walls, start, end);
    // print_grid(&size, &path, &walls);

    let cheat_length = 20;
    find_cheats(&path, cheat_length);
    return 0;
    // let cheats = find_cheats(&path, cheat_length);
    // println!("{:?}", cheats);

    // let result = cheats.len();
    // return result;
}

pub fn main() {
    let sample = include_str!("day20.sample").trim();
    validate::eq(41, solve(sample, 30), "day20 part01 sample");

    let input = include_str!("day20.input").trim();
    validate::eq(0, solve(input, 100), "day20 part01 input");
}
