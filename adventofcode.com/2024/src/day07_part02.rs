use itertools::Itertools;
use nom::{
    bytes::complete::tag,
    character::complete::{self, line_ending, space1},
    multi::separated_list1,
    sequence::separated_pair,
    IResult,
};

use super::validate;

fn parse(input: &str) -> IResult<&str, Vec<(u64, Vec<u64>)>> {
    separated_list1(
        line_ending,
        separated_pair(
            complete::u64,
            tag(": "),
            separated_list1(space1, complete::u64),
        ),
    )(input)
}

#[derive(Clone, Debug, PartialEq)]
enum Op {
    Add,
    Mul,
    Con,
}

type Ops = Vec<Op>;

fn advance(ops: &mut Ops) -> bool {

    let mut i: usize = 0;
    while i < ops.len() {
        if ops[i] == Op::Add {
            ops[i] = Op::Mul;
            return true;
        } else if ops[i] == Op::Mul {
            ops[i] = Op::Con;
            return true;
        } 

        ops[i] = Op::Add;
        i += 1;
    }
    return false;
}

fn apply(a: &u64, b: &u64, op: &Op) -> u64 {
    if op == &Op::Add {
        return a + b;
    } if op == &Op::Mul {
        return a * b;
    }

    // a || b = ab
    // 640 || 12 = 64012
    // 12.ilog(10) + 1 = 2
    // u64::pow(10, c) = 100
    // r = 640 * 100 + 12 = 64012
    let c = b.ilog(10) + 1;
    let d = u64::pow(10, c);
    let r = a * d + b;
    return r
}



fn correct_number(value: &u64, parts: &Vec<u64>) -> bool {
    let mut ops: Ops = vec!(Op::Add; parts.len() - 1);
    loop {
        let mut result: u64 = 0;
        for (i, (a, b)) in parts.iter().tuple_windows().enumerate() {
            if result == 0 {
                result = *a;
            }

            result = apply(&result, b, &ops[i]);

            if &result > value {
                break;
            }
        }

        if &result == value {
            return true;
        }

        if !advance(&mut ops) {
            return false;
        }
    }
}

fn solve(input: &str) -> u64 {
    let (_input, data) = parse(input).unwrap();

    let correct_numbers = data
        .iter()
        .filter(|(value, parts)| correct_number(value, parts))
        .map(|(value, _)| value).collect_vec();

    return correct_numbers.into_iter().sum();
}

pub fn main() {
    let sample = include_str!("day07.sample");
    validate::eq(11387, solve(sample), "day07 part02 sample");

    let input = include_str!("day07.input");
    validate::eq(438027111276610, solve(input), "day07 part02 input");
}
