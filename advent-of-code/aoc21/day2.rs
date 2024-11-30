use std::fs;

pub fn run() {
    let moves = fs::read_to_string("aoc21/input/day2.txt").expect("Failed to open file");
    println!(
        "=> Day 2 part a: depth * horizontal = {} (expected {})",
        move_submarine(&moves),
        1947824
    );
}

/// Given a string containing the moves, returns the product between depth and
/// horizontal positions
fn move_submarine(moves: &str) -> i32 {
    let (d, h, _) = moves.lines().map(|m| m.split_whitespace()).fold(
        (0, 0, 0),
        |(depth, horiz, aim), mut m| {
            let mv = |s: &Option<&str>| match s {
                Some(x) => x.parse::<i32>().unwrap(),
                None => panic!("Not able to move this amount"),
            };

            match m.next() {
                Some("forward") => {
                    let x = m.next();
                    (depth + aim * mv(&x), horiz + mv(&x), aim)
                }
                Some("down") => (depth, horiz, aim + mv(&m.next())),
                Some("up") => (depth, horiz, aim - mv(&m.next())),
                _ => panic!("Not able to move in this direction"),
            }
        },
    );
    d * h
}
