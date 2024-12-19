use super::validate;
use glam::IVec2;
use itertools::Itertools;
use nom::{
    bytes::complete::take_till, character::complete::satisfy, multi::many1, sequence::preceded,
    IResult, AsChar,
};
use nom_locate::{position, LocatedSpan};

pub type Span<'a> = LocatedSpan<&'a str>;

fn alphanum_pos(input: Span) -> IResult<Span, (IVec2, char)> {
    let (input, pos) = position(input)?;
    let x = pos.get_column() as i32 - 1;
    let y = pos.location_line() as i32 - 1;
    let (input, c) = satisfy(|c| c.is_alphanum())(input)?;
    Ok((input, (IVec2::new(x, y), c)))
}
pub fn parse(input: Span) -> IResult<Span, Vec<(IVec2, char)>> {
    many1(preceded(take_till(|c: char| c.is_alphanum()), alphanum_pos))(input)
}

fn solve(input: &str) -> usize {
    let height = input.lines().count();
    let width = input.lines().next().unwrap().len();

    let x_bound = 0i32..width as i32;
    let y_bound = 0i32..height as i32;

    let (_input, mut result) = parse(Span::new(input)).unwrap();

    result.sort_by(|a, b| a.1.cmp(&b.1));
    let results = result
        .chunk_by(|a, b| a.1 == b.1)
        .flat_map(|chunk| {
            chunk.iter()
            .combinations(2)
            .flat_map(|sats| {
                let diff = sats[0].0 - sats[1].0;
                [sats[0].0 + diff, sats[1].0 - diff]
            })
        })
        .filter(|pos|{
            x_bound.contains(&pos.x) && y_bound.contains(&pos.y)
        })
        .unique()
        .count();

    return results;
}

pub fn main() {
    let sample = include_str!("day08.sample");
    validate::eq(14, solve(sample), "day08 part01 sample");

    let input = include_str!("day08.input");
    validate::eq(371, solve(input), "day08 part01 input");
}
