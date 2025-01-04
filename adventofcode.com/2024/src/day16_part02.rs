use itertools::Itertools;
use pathfinding::prelude::astar_bag_collect;
use std::collections::HashSet;

use glam::IVec2;

use super::validate;

type Maze = HashSet<IVec2>;

fn parse_maze(input: &str) -> (Maze, IVec2, IVec2, IVec2) {
    let mut maze: Maze = Maze::default();
    let mut start: IVec2 = IVec2::ZERO;
    let mut end: IVec2 = IVec2::ZERO;

    let lines = input.lines().collect_vec();
    let size: IVec2 = IVec2::new(lines.len() as i32, lines[0].len() as i32);

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
        }
    }

    return (maze, size, start, end);
}

#[derive(Clone, Debug, Eq, Hash, PartialEq)]
struct Node {
    pos: IVec2,
    dir: IVec2,
}

fn get_neighbours(maze: &Maze, node: &Node) -> Vec<(Node, usize)> {
    let mut neighbours: Vec<(Node, usize)> = Vec::default();

    neighbours.push((
        Node {
            pos: node.pos,
            dir: node.dir.perp(),
        },
        1000,
    ));
    neighbours.push((
        Node {
            pos: node.pos,
            dir: -node.dir.perp(),
        },
        1000,
    ));

    let p = node.pos + node.dir;
    if maze.contains(&p) == false {
        neighbours.push((
            Node {
                pos: p,
                dir: node.dir,
            },
            1,
        ));
    }

    return neighbours;
}

fn _print_solutions(maze: &Maze, size: &IVec2, paths: &HashSet<IVec2>) {

    for y in 0..size.y {
        for x in 0..size.x {
            let p = IVec2::new(x, y);
            let mut c = ' ';
            if maze.contains(&p) {
                c = '#';
            } else if paths.contains(&p) {
                c = 'O';
            }
            print!("{}", &c);
        }
        println!();
    }
}

fn solve(input: &str) -> usize {
    let (maze, _size, start, end) = parse_maze(input);

    let result = astar_bag_collect(
        &Node {
            pos: start,
            dir: IVec2::new(1, 0),
        },
        |p| get_neighbours(&maze, p),
        |_p| 0,
        |p| p.pos == end,
    );
    let solutions = result.unwrap();
    
    // println!("Found {} paths with cost {}", solutions.0.len(), &solutions.1);

    let mut nodes: HashSet<IVec2> = HashSet::default();
    for path in solutions.0 {
        for node in path {
            nodes.insert(node.pos);
        }
    }

    // print_solutions(&maze, &size, &nodes);
    // println!("{:?}", nodes);

    return nodes.len();
}

pub fn main() {
    let sample = include_str!("day16_1.sample").trim();
    validate::eq(45, solve(sample), "day16 part02 sample 1");

    let sample = include_str!("day16_2.sample").trim();
    validate::eq(64, solve(sample), "day16 part02 sample 2");

    let input = include_str!("day16.input").trim();
    validate::eq(535, solve(input), "day16 part02 input");
}
