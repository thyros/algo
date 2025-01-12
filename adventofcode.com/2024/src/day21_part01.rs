use std::{collections::{HashMap, VecDeque}, i32};

use glam::IVec2;
use itertools::Itertools;

use super::validate;

fn generate_possible_sequences(width: i32, height: i32, target: char, pos: IVec2, keypad: &Vec<Vec<char>>) -> Vec<String> {
    let mut q: VecDeque<(IVec2, String)> = VecDeque::default();
    let mut optimal_length: usize = usize::MAX;
    let mut possibilities: Vec<String> = Vec::default();

    q.push_back((pos, "".to_string()));

    while !q.is_empty() {
        let (p, moves) = q.pop_front().unwrap();

        for (np, nm) in vec!((p + IVec2::X, ">"),(p - IVec2::Y, "^"),
                (p - IVec2::X, "<"),(p + IVec2::Y, "v")) {
            
            if np.x < 0 || np.y < 0 || np.x >= width || np.y >= height { continue; }
            if keypad[np.y as usize][np.x as usize] == ' ' { continue; }
            if keypad[np.y as usize][np.x as usize] == target {
                if optimal_length < moves.len() + 1 {
                    return possibilities
                }

                optimal_length = moves.len() + 1;
                possibilities.push(moves.to_owned() + nm + "A")
            } else {
                q.push_back((np, moves.to_owned() + nm));
            }
        }
    }

    return possibilities;
}

fn generate_seqs(width: i32, height: i32, pos: &HashMap<char, IVec2>, keypad: &Vec<Vec<char>>) -> HashMap<(char, char), Vec<String>> {
    let mut seqs: HashMap<(char, char), Vec<String>> = HashMap::default();

    for (&lc, &lp) in pos {
        for (&rc, &_rp) in pos {
            if lc == rc {
                seqs.insert((lc, rc), vec!("A".to_string()));            
                continue;
            }
            seqs.insert((lc, rc), generate_possible_sequences(width, height, rc, lp, keypad));
        }
    }

    return seqs;
}

fn generate_solutions(input: &str, seqs: &HashMap<(char, char), Vec<String>>) -> Vec<String> {
    dbg!(input, seqs);
    let possible_steps = input.chars().tuple_windows::<(char, char)>().map(|p| seqs[&p].clone()).collect_vec();
    // dbg!(&possible_steps);

    let mut solutions: Vec<String> = possible_steps[0].clone();
    for s in possible_steps.iter().skip(1) {
        let next = solutions.iter().cartesian_product(s.iter()).map(|(a,b)| format!("{}{}", a, b)).collect_vec();
        solutions = next;
    }

    return solutions;
    // dbg!(solutions);
}

fn generate_moves(input: String, keypad: Vec<Vec<char>>, dir_keypad: Vec<Vec<char>>) -> i32 {
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

    let mut total: i32 = 0;

    let seqs = generate_seqs(width, height, &pos, &keypad);
    for line in input.lines() {
        let full_line = "A".to_string() + line;

        let mut next = generate_solutions(&full_line, &seqs);

        for _ in 0..2 {
            let mut possible_next: Vec<String> = Vec::default();

            for seq in next {
                possible_next.extend_from_slice(&generate_solutions(&seq, &seqs));
            }
            let minlen = possible_next.iter().map(|n| n.len()).min().unwrap();
            next = possible_next.into_iter().filter(|n| n.len() == minlen).collect_vec();
        }

        let length = next[0].len() as i32;
        let i = input[..input.len() -1].parse::<i32>().unwrap();
        let complexity  = length * i;

        total += complexity;
    }
    return total;
}

fn solve(input: &str) -> usize {
    let num_keypad = vec!(
        vec!('7', '8', '9'),
        vec!('4', '5', '6'),
        vec!('1', '2', '3'),
        vec!(' ', '0', 'A'),
    );

    let dir_keypad = vec!(
        vec!(' ', '^', 'A'),
        vec!('<', 'v', '>'),
    );

    let moves = generate_moves(input.to_string(), num_keypad, dir_keypad);
    dbg!(moves);

    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day21.input").trim();
    validate::eq(126384, solve(sample), "day21 part01 sample");

    let input = include_str!("day21.input").trim();
    validate::eq(0, solve(input), "day21 part01 input");
}