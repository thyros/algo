
use std::{
    collections::{HashMap, VecDeque},
    i32,
};

use glam::IVec2;
use itertools::Itertools;

use super::validate;

// map transition between two buttons to possible set of moves to reach from one button to another
type Transitions = HashMap<(char, char), Vec<String>>;

fn generate_transitions(keypad: &Vec<Vec<char>>) -> Transitions {
    let height: i32 = keypad.len() as i32;
    let width: i32 = keypad[0].len() as i32;

    let mut pos: HashMap<char, IVec2> = HashMap::default();
    for (y, row) in keypad.iter().enumerate() {
        for (x, &c) in row.iter().enumerate() {
            if c != ' ' {
                pos.insert(c, IVec2::new(x as i32, y as i32));
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
    width: i32,
    height: i32,
    target: char,
    pos: IVec2,
    keypad: &Vec<Vec<char>>,
) -> Vec<String> {
    let mut q: VecDeque<(IVec2, String)> = VecDeque::default();
    let mut optimal_length: usize = usize::MAX;
    let mut transitions: Vec<String> = Vec::default();

    q.push_back((pos, "".to_string()));

    while !q.is_empty() {
        let (p, moves) = q.pop_front().unwrap();

        for (np, nm) in vec![
            (p + IVec2::X, ">"),
            (p - IVec2::Y, "^"),
            (p - IVec2::X, "<"),
            (p + IVec2::Y, "v"),
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

fn solve(input: &str) -> i32 {
    let num_keypad = vec![
        vec!['7', '8', '9'],
        vec!['4', '5', '6'],
        vec!['1', '2', '3'],
        vec![' ', '0', 'A'],
    ];
    let num_transitions = generate_transitions(&num_keypad);

    let dir_keypad = vec![
        vec![' ', '^', 'A'], 
        vec!['<', 'v', '>']];
    let dir_transitions = generate_transitions(&dir_keypad);

    let mut result: i32 = 0;
    for line in input.lines() {
        let mut next = generate_solutions(&line, &num_transitions);

        for _ in 0..2 {
            let mut possible_next: Vec<String> = Vec::default();

            for transition in next {
                let transitions = generate_solutions(&transition, &dir_transitions);
                possible_next.extend_from_slice(&transitions);
            }

            let minlen = possible_next.iter().map(|n| n.len()).min().unwrap();            
            next = possible_next
                .into_iter()
                .filter(|n| n.len() == minlen)
                .collect_vec();

        }

        let length = next[0].len() as i32;
        let s = &line[..line.len() - 1];
        let i = s.parse::<i32>().unwrap();
        let complexity = length * i;


        result += complexity;
    }
    return result;
}

pub fn main() {
    let sample = include_str!("day21.sample").trim();
    validate::eq(126384, solve(sample), "day21 part02 sample");

    let input = include_str!("day21.input").trim();
    validate::eq(171596, solve(input), "day21 part02 input");
}
