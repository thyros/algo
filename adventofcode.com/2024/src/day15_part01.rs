use std::collections::HashSet;

use glam::IVec2;
use itertools::Itertools;

use super::validate;
#[derive(Debug)]
struct Warehouse {
    walls: HashSet<IVec2>,
    boxes: HashSet<IVec2>,
    robot: IVec2,
    size: IVec2,
}

fn parse_warehouse(input: &str) -> Warehouse {
    let mut walls: HashSet<IVec2> = HashSet::default();
    let mut boxes: HashSet<IVec2> = HashSet::default();
    let mut robot: IVec2 = IVec2::ZERO;

    let lines = input.lines().collect_vec();
    let size: IVec2 = IVec2::new(lines.len() as i32, lines[0].len() as i32);
    
    for (y, line) in input.lines().enumerate() {
        for (x, c) in line.chars().enumerate() {
            let p = IVec2::new(x as i32, y as i32);
            match c {
                '#' => {walls.insert(p);}
                'O' => {boxes.insert(p);}
                '@' => {robot = p; }
                _ => {}
            }
        }
    }

    return Warehouse{ walls, boxes, robot, size}; 
}

fn parse_commands(input: &str) -> String {
    input.replace("\r\n", "")
}

fn get_obj (pos: &IVec2, walls: &HashSet<IVec2>, size: &IVec2, boxes: &HashSet<IVec2>, robot: &IVec2) -> char {
    if pos.x < 0 || pos.y < 0 || pos.x >= size.x || pos.y >= size.y {
        return '#';
    } else if walls.contains(pos) {
        return '#';
    } else if boxes.contains(pos) {
        return 'O';
    } else if robot == pos {
        return '@'
    }
    return '.';
}

fn _print_warehouse(walls: &HashSet<IVec2>, size: &IVec2, boxes: &HashSet<IVec2>, robot: &IVec2) {
    for y in 0..size.y {
        for x in 0..size.x {
            print!("{}", get_obj(&IVec2::new(x, y), walls, size, boxes, robot));            
        }
        println!();
    }
}

fn push(walls: &HashSet<IVec2>, size: &IVec2, boxes: &mut HashSet<IVec2>, robot: &mut IVec2, dir: IVec2) {
    let a_pos = *robot + dir;
    if get_obj(&a_pos, &walls, &size, &boxes, &robot) == '.' {
        *robot = a_pos;
        return;
    }

    let mut b_pos = a_pos;
    loop {
        let obj = get_obj(&b_pos, &walls, &size, &boxes, &robot);
        match obj {
            '#' => { break; }
            'O' => { b_pos += dir; }
            '.' => {
                boxes.insert(b_pos);
                boxes.remove(&a_pos);
                *robot = a_pos;
                break;
            }
            _ => { panic!("Unrecognized command"); }
        }
    }
}

fn simulate(walls: &HashSet<IVec2>, size: &IVec2, mut boxes: HashSet<IVec2>, mut robot: IVec2, commands: &str) -> HashSet<IVec2> {
    // print_warehouse(&walls, &size, &boxes, &robot);
    for c in commands.chars() {
        // println!("Command: {}", c);
        match c {
            '<' => { push(&walls, &size, &mut boxes, &mut robot, IVec2::new(-1, 0)); }
            '>' => { push(&walls, &size, &mut boxes, &mut robot, IVec2::new(1, 0)); }
            '^' => { push(&walls, &size, &mut boxes, &mut robot, IVec2::new(0, -1)); }
            'v' => { push(&walls, &size, &mut boxes, &mut robot, IVec2::new(0, 1)); }
            _ => { panic!("Unrecongnized command"); }
        }
        // print_warehouse(&walls, &size, &boxes, &robot);
    }

    return boxes;
}

fn parse(input: &str) -> (Warehouse, String) {

    let parts = input.split("\r\n\r\n").collect_vec();
    let warehouse = parse_warehouse(parts[0]);
    let commands = parse_commands(parts[1]);

    return (warehouse, commands);
}

fn count_result(boxes: HashSet<IVec2>)  -> i32 {
    boxes.iter().map(|b| {
        return 100 * b.y + b.x;
    }).sum()
}

fn solve(input: &str) -> i32 {
    let (warehouse, commands) = parse(input);
    // println!("{:?}\n{:?}", &warehouse, &commands);

    let boxes = simulate(&warehouse.walls, &warehouse.size, warehouse.boxes, warehouse.robot, &commands);

    let result = count_result(boxes);
    return result;
}

pub fn main() {
    let sample = include_str!("day15_1.sample").trim();
    validate::eq(2028, solve(sample), "day15 part01 sample 1");

    let sample = include_str!("day15_2.sample").trim();
    validate::eq(10092, solve(sample), "day15 part01 sample 2");

    let input = include_str!("day15.input").trim();
    validate::eq(1487337, solve(input), "day15 part01 input");
}
