use pathfinding::prelude::dijkstra;
use std::collections::HashSet;

use glam::IVec2;

use super::validate;

type Maze = HashSet<IVec2>;

fn parse_maze(input: &str) -> (Maze, IVec2, IVec2) {
    let mut maze: Maze = Maze::default();
    let mut start: IVec2 = IVec2::ZERO;
    let mut end: IVec2 = IVec2::ZERO;

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

    return (maze, start, end);
}

#[derive(Clone, Debug, Eq, Hash, PartialEq)]
struct Node {
    pos: IVec2,
    dir: IVec2,
}

fn get_neighbours(maze: &Maze, node: &Node) -> Vec<(Node, usize)> {
    let mut neighbours: Vec<(Node, usize)> = Vec::default();

    neighbours.push( (Node {pos: node.pos, dir: node.dir.perp()}, 1000));
    neighbours.push( (Node {pos: node.pos, dir: -node.dir.perp()}, 1000));

    let p = node.pos + node.dir;
    if maze.contains(&p) == false {
        neighbours.push( (Node {pos: p, dir: node.dir}, 1));
    }

    return neighbours;
}

fn solve(input: &str) -> usize {
    let (maze, start, end) = parse_maze(input);

    let result = dijkstra(
        &Node { pos: start, dir: IVec2::new(1, 0) },
        |p| { get_neighbours(&maze, p)},
        |p| {
            p.pos == end
        },
    );

    // dbg!(&result);

    return result.unwrap().1;
}

pub fn main() {
    let sample = include_str!("day16_1.sample").trim();
    validate::eq(7036, solve(sample), "day16 part01 sample 1");

    let sample = include_str!("day16_2.sample").trim();
    validate::eq(11048, solve(sample), "day16 part01 sample 2");

    let input = include_str!("day16.input").trim();
    validate::eq(102504, solve(input), "day16 part01 input");
}
