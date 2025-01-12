use std::{
    collections::{HashMap, VecDeque},
    i64,
};

use glam::I64Vec2;
use itertools::Itertools;

use super::validate;

// map transition between two buttons to possible set of moves to reach from one button to another
type Transitions = HashMap<(char, char), Vec<String>>;

fn generate_transitions(keypad: &Vec<Vec<char>>) -> Transitions {
    let height: i64 = keypad.len() as i64;
    let width: i64 = keypad[0].len() as i64;

    let mut pos: HashMap<char, I64Vec2> = HashMap::default();
    for (y, row) in keypad.iter().enumerate() {
        for (x, &c) in row.iter().enumerate() {
            if c != ' ' {
                pos.insert(c, I64Vec2::new(x as i64, y as i64));
            }
        }
    }

    let mut transitions: Transitions = HashMap::default();

    for (&lc, &lp) in &pos {
        for (&rc, &_rp) in &pos {
            if lc == rc {
                transitions.insert((lc, rc), vec!["A".to_string()]);
                continue;
            }
            transitions.insert(
                (lc, rc),
                generate_possible_transitions(width, height, rc, lp, keypad),
            );
        }
    }

    return transitions;
}

// all possible ways of moving from pos to target
fn generate_possible_transitions(
    width: i64,
    height: i64,
    target: char,
    pos: I64Vec2,
    keypad: &Vec<Vec<char>>,
) -> Vec<String> {
    let mut q: VecDeque<(I64Vec2, String)> = VecDeque::default();
    let mut optimal_length: usize = usize::MAX;
    let mut transitions: Vec<String> = Vec::default();

    q.push_back((pos, "".to_string()));

    while !q.is_empty() {
        let (p, moves) = q.pop_front().unwrap();

        for (np, nm) in vec![
            (p + I64Vec2::X, ">"),
            (p - I64Vec2::Y, "^"),
            (p - I64Vec2::X, "<"),
            (p + I64Vec2::Y, "v"),
        ] {
            if np.x < 0 || np.y < 0 || np.x >= width || np.y >= height {
                continue;
            }
            if keypad[np.y as usize][np.x as usize] == ' ' {
                continue;
            }
            if keypad[np.y as usize][np.x as usize] == target {
                if optimal_length < moves.len() + 1 {
                    return transitions;
                }

                optimal_length = moves.len() + 1;
                transitions.push(moves.to_owned() + nm + "A")
            } else {
                q.push_back((np, moves.to_owned() + nm));
            }
        }
    }

    return transitions;
}

fn generate_solutions(input: &str, transitions: &Transitions) -> Vec<String> {
    let full_line = "A".to_string() + input;

    let possible_steps = full_line
        .chars()
        .tuple_windows::<(char, char)>()
        // .inspect(|p| { println!("{:?}->{:?}", &p, &transitions[&p]); })
        .map(|p| transitions[&p].clone())
        .collect_vec();

    let mut solutions: Vec<String> = possible_steps[0].clone();
    for s in possible_steps.iter().skip(1) {
        let next = solutions
            .iter()
            .cartesian_product(s.iter())
            .map(|(a, b)| format!("{}{}", a, b))
            .collect_vec();
        solutions = next;
    }

    return solutions;
}

type Cache = HashMap<(char, char, i64), i64>;

fn raw_calculate_length(
    x: char,
    y: char,
    depth: i64,
    transitions: &Transitions,
    dir_lengths: &HashMap<(char, char), i64>,
    cache: &mut Cache,
) -> i64 {
    if depth == 1 {
        return dir_lengths[&(x, y)];
    }

    let mut optimal = i64::MAX;
    for transition in &transitions[&(x, y)] {
        let mut length = 0;
        // dbg!(transition);
        for (a, b) in ("A".to_string() + &transition).chars().tuple_windows() {
            let sub_lenth = calculate_length(a, b, depth - 1, transitions, dir_lengths, cache);
            // println!("({},{}) -> {}", &a, &b, &sub_lenth);
            length += sub_lenth;
        }
        optimal = optimal.min(length);
    }
    return optimal;
}

fn calculate_length(
    x: char,
    y: char,
    depth: i64,
    transitions: &Transitions,
    dir_lengths: &HashMap<(char, char), i64>,
    cache: &mut Cache,
) -> i64 {
    let cached = cache.get(&(x, y, depth));
    if cached.is_some() {
        return cached.unwrap().clone();
    }

    let result = raw_calculate_length(x, y, depth, transitions, dir_lengths, cache);
    cache.insert((x, y, depth), result);
    return result;
}

fn solve(input: &str) -> i64 {
    let num_keypad = vec![
        vec!['7', '8', '9'],
        vec!['4', '5', '6'],
        vec!['1', '2', '3'],
        vec![' ', '0', 'A'],
    ];
    let num_transitions = generate_transitions(&num_keypad);

    let dir_keypad = vec![vec![' ', '^', 'A'], vec!['<', 'v', '>']];
    let dir_transitions = generate_transitions(&dir_keypad);

    let mut dir_lengths: HashMap<(char, char), i64> = HashMap::default();
    for (k, v) in dir_transitions.iter() {
        dir_lengths.insert(k.clone(), v[0].len() as i64);
    }

    print!("{:?}", dir_lengths);

    let mut cache: Cache = Cache::default();

    let mut result: i64 = 0;
    for line in input.lines() {
        let next = generate_solutions(&line, &num_transitions);

        let mut optimal = i64::MAX;
        for transition in next {
            let mut length = 0;
            for (x, y) in ("A".to_string() + &transition).chars().tuple_windows() {
                length += calculate_length(x, y, 25, &dir_transitions, &dir_lengths, &mut cache)
            }
            optimal = optimal.min(length);
        }

        let s = &line[..line.len() - 1];
        let i = s.parse::<i64>().unwrap();
        let complexity = optimal * i;
        result += complexity;
    }
    return result;
}

pub fn main() {
    let sample = include_str!("day21.sample").trim();
    validate::eq(154115708116294, solve(sample), "day21 part02 sample");

    let input = include_str!("day21.input").trim();
    validate::eq(209268004868246, solve(input), "day21 part02 input");
}
