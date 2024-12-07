use super::validate;
use itertools::Itertools;
use nom::{
    character::complete::{self, newline, space1},
    multi::separated_list1,
    IResult,
};

#[derive(Debug, PartialEq)]
enum Safety {
    Safe,
    Unsafe,
}

enum Direction {
    Increasing,
    Decreasing,
}

type Report = Vec<i32>;

fn parse(input: &str) -> IResult<&str, Vec<Report>> {
    separated_list1(
        newline,
        separated_list1(space1, complete::i32),
    )(input)
}

fn check_safety(report: &Report) -> Safety {
    let mut direction: Option<Direction> = None;

    for (l, r) in report.iter().tuple_windows() {
        let diff = l - r;
        if !(1..=3).contains(&diff.abs()) {
            return Safety::Unsafe;
        }

        match diff.signum() {
            -1 => match direction {
                Some(Direction::Increasing) => {
                    return Safety::Unsafe;
                }
                Some(Direction::Decreasing) => { 
                    continue;
                }
                None => {
                    direction = Some(Direction::Decreasing);
                }
            }
            1 => match direction {
                Some(Direction::Increasing) => {
                    continue;
                }
                Some(Direction::Decreasing) => {
                    return Safety::Unsafe;
                }
                None => {
                    direction = Some(Direction::Increasing);
                }
            }
            0 => {
                return Safety::Unsafe;
            }
            _ => {
                panic!(
                    "should never have a non 0, 1, -1 number"
                );
            }
        }
    }
    return Safety::Safe;
}


fn solve1(input: &str) -> usize {
    let (_, reports) = parse(input).unwrap();
    let result = reports
        .iter()
        .map(|report| check_safety(report))
        .filter(|safety| safety == &Safety::Safe )
        .count();

    return result;
}

fn solve2(input: &str) -> usize {
    let (_, reports) = parse(input).unwrap();
    let result = reports
        .iter()
        .filter(|report| {
            if check_safety(report) == Safety::Unsafe {
                for index in 0..report.len() {
                    let mut new_report = (*report).clone();
                    new_report.remove(index);
                    if check_safety(&new_report) == Safety::Safe {
                        return true;
                    } else {
                        continue;
                    }
                }
                return false;
            } else {
                return true;
            }
        })
        .count();

    return result;
}

pub fn solve() {
    let sample = include_str!("day02.sample");
    validate::eq(2, solve1(sample));
    validate::eq(4, solve2(sample));

    let input = include_str!("day02.input");
    validate::eq(257, solve1(input));
    validate::eq(328, solve2(input));
}