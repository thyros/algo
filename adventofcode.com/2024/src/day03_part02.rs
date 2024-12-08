use super::validate;

use nom::{
    branch::alt,
    bytes::complete::tag,
    character::complete,
    character::complete::anychar,
    combinator::{value},
    multi::{many1, many_till},
    Parser,
    sequence::{delimited, separated_pair},    
    IResult
};

#[derive(Debug, Clone)]
enum Instruction {
    Mul(u32, u32),
    Do,
    Dont,
}

#[derive(PartialEq, Eq)]
enum ShouldProcess {
    Do,
    Dont
}

fn mul(input: &str) -> IResult<&str, Instruction> {
    let (input, _) = tag("mul")(input)?;
    let (input, pair) = delimited(
        tag("("),
        separated_pair(
            complete::u32,
            tag(","),
            complete::u32,
        ),
        tag(")"),   
    )(input)?;

    let mul = Instruction::Mul(pair.0, pair.1);
    return Ok((input, mul));
}

fn instruction(input: &str) -> IResult<&str, Instruction> {
    alt((
        value(Instruction::Dont, tag("don't()")),
        value(Instruction::Do, tag("do()")),
        mul,
    ))(input)
}

fn parse(input: &str) -> IResult<&str, Vec<Instruction>> {
    many1(
        many_till(anychar, instruction)
            .map(|(_discard, ins)| ins)
    )(input)
}

fn solve(input: &str) -> u32 {
    let (_, instructions) = parse(input).unwrap();

    let (_, result) = instructions.iter().fold(
        (ShouldProcess::Do, 0),
        |(process, acc), ins| match ins {
            Instruction::Mul(a, b) => {
                if process == ShouldProcess::Do {
                    (process, acc + a * b)
                } else {
                    (process, acc)
                }
            },
            Instruction::Do => {
                (ShouldProcess::Do, acc)
            },
            Instruction::Dont => {
                (ShouldProcess::Dont, acc)
            }
        },
    );

    return result;
}

pub fn main() {
    let sample = include_str!("day03_part02.sample");
    validate::eq(48, solve(sample), "day03 part02 sample");

    let input = include_str!("day03.input");
    validate::eq(83595109, solve(input), "day03 part02 input");
}