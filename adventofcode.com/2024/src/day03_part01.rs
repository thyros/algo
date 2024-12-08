use super::validate;

use nom::{
    bytes::complete::tag,
    character::complete,
    character::complete::anychar,
    multi::{many1, many_till},
    Parser,
    sequence::{delimited, separated_pair},    
    IResult
};

#[derive(Debug, Clone)]
enum Instruction {
    Mul(u32, u32),
}


fn instruction(input: &str) -> IResult<&str, Instruction> {
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

fn parse(input: &str) -> IResult<&str, Vec<Instruction>> {
    many1(
        many_till(anychar, instruction)
            .map(|(_discard, ins)| ins)
    )(input)
}

fn solve(input: &str) -> u32 {
    let (_, instructions) = parse(input).unwrap();

    let result: u32 = instructions
        .iter()
        .map(|i| match i {
            Instruction::Mul(a, b) => a * b })       
        .sum();

    return result;
}

pub fn main() {
    let sample = include_str!("day03_part01.sample");
    validate::eq(161, solve(sample),"day03 part01 sample");

    let input = include_str!("day03.input");
    validate::eq(161289189, solve(input),"day03 part01 input");
}