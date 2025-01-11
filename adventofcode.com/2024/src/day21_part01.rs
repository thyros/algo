use std::{collections::{vec_deque, HashMap, VecDeque}, i32};

use glam::IVec2;

use super::validate;


fn generate_seqs(width: i32, height: i32, pos: &HashMap<char, IVec2>, keypad: &Vec<Vec<char>>) -> HashMap<(char, char), Vec<String>> {
    let mut seqs: HashMap<(char, char), Vec<String>> = HashMap::default();

    for (&lc, &lp) in pos {
        for (&rc, &rp) in pos {
            if lc == rc {
                seqs.insert((lc, rc), vec!("A".to_string()));            
                continue;
            }

            let mut q: VecDeque<(IVec2, String)> = VecDeque::default();
            let mut optimal_length: usize = usize::MAX;
            let mut possibilities: Vec<String> = Vec::default();

            q.push_back((lp, "".to_string()));

            while !q.is_empty() {
                let (p, moves) = q.pop_front().unwrap();

                for (np, nm) in vec!((p + IVec2::X, ">"),(p - IVec2::Y, "^"),
                        (p - IVec2::X, "<"),(p + IVec2::Y, "v")) {
                    
                    if np.x < 0 || np.y < 0 || np.x >= width || np.y >= height { continue; }
                    if keypad[np.y as usize][np.x as usize] == ' ' { continue; }
                    if keypad[np.y as usize][np.x as usize] == rc {
                        if optimal_length < moves.len() + 1 {
                            return seqs
                        }

                        optimal_length = moves.len() + 1;
                        possibilities.push(moves.to_owned() + nm + "A")
                    } else {
                        q.push_back((np, moves.to_owned() + nm));
                    }
                }
            }
            seqs.insert((lc, rc), possibilities);
        }
    }

    return seqs;
}

fn generate_moves(input: &str, keypad: Vec<Vec<char>>) -> String {
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

    let seqs = generate_seqs(width, height, &pos, &keypad);


    dbg!(seqs);

    return "".to_string();
}


fn solve(_input: &str) -> usize {

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

    let moves = generate_moves("029A", num_keypad);
    dbg!(moves);

    let result: usize = 0;
    return result;
}

pub fn main() {
    let sample = include_str!("day21.sample").trim();
    validate::eq(126384, solve(sample), "day21 part01 sample");

    let input = include_str!("day21.input").trim();
    validate::eq(0, solve(input), "day21 part01 input");
}