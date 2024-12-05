use nom::multi::separated_list1;

#[derive(PartialEq)]
enum Safety {
    Safe,
    Unsafe,
};

enum Direction {
    Increasing,
    Decreasing,
};

type Report = Vec<i32>;

fn parse(input: &str) -> IResult<&str, Report> {
    separated_list1(
        newline,
        separated_list1(space1, complete::i32),
    )(input)
}

fn check_safety(report: Report) -> Safety {
    let mut direction: Option<Direction> = None;

    for (l, r) in report.iter().tuple_windows() {
        let diff = l - r;
        match diff.signum() {
            -1 => match direction {
                Some(Direction::Increasing) => {
                    return Safety::Unsafe;
                }
                Some(Direction::Decreasing) => {
                    if !(1..3).contains(&diff.abs()) {
                        return Safety::Unsafe;
                    } else {
                        continue;
                    }
                }
                None => {
                    direction = Some(Direction::Increasing);
                }
            }
            1 => match direction {
                Some(Direction::Decreasing) => {
                    if !(1..3).contains(&diff.abs()) {
                        return Safety::Unsafe;
                    } else {
                        continue;
                    }
                }
                Some(Direction::Increasing) => {
                    return Safety::Unsafe;
                }
                None => {
                    direction = Some(Direction::Decreasing);
                }
            }
            0 => {
                return Safety::Unsafe;
            }
        }
    }

    return Safety::Safe;
}


fn solve1(input: &str) -> i32 {
    let (_, reports) = parse(input)
    let result = reports
        .iter()
        .filter(|report| {
            check_safety(report) == Safety::Safe
        })
        .count();

    return result;
}

fn solve2(_input: &str) -> i32 {
    return 0;
}

fn main() {
    let file = include_str!("day02.sample");
    // let file = include_str!("day02.input");
    println!("Result part 1: {}", solve1(file));
    println!("Result part 2: {}", solve2(file));
}