fn solve1(input: &str) -> i32 {
    let mut total: i32 = 0;
    let mut count: i32 = 0;

    for line in input.lines() {
        total += 1;
        let mut v = vec![];
        let split = line.split_whitespace();

        split.for_each(|x| { v.push(x.parse::<i32>().unwrap())});

        let mut left: i32 = v[0];
        let mut right: i32 = v[1];
        let mut diff: i32 = right - left;
        if diff.abs() < 1 && diff.abs() > 3 {
            count += 1;
        }

        for i in 2..v.len() {
            left = right;
            right = v[i];

            let ndiff = right - left;
            println!("loop {} {}", i, ndiff);
            if ndiff.abs() < 1 || ndiff.abs() > 3 {             
                println!("\tdelta {} {} {}", left, right, ndiff.abs());
                count += 1;
                break;
            }
            if diff * ndiff < 0 {
                println!("\tdiff {} {}", diff, ndiff);
                count += 1;
                break;
            }
            diff = ndiff;            
        }

        println!("");
    }

    println!("{} - {} = {}", total, count, (total - count));
    return total - count;
}

fn solve2(_input: &str) -> i32 {
    return 0;
}

fn main() {
    // let file = include_str!("day02.sample");
    let file = include_str!("day02.input");
    println!("Result part 1: {}", solve1(file));
    println!("Result part 2: {}", solve2(file));
}