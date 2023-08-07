use std::fs;

pub fn run() {
    let depths: Vec<i32> = fs::read_to_string("input/day1.txt")
        .expect("Failed to open file")
        .lines()
        .map(|x| x.parse::<i32>().unwrap())
        .collect();

    println!(
        "=> Day 1 part a: The increase count is {} (expected {})",
        count_depth_inc(&depths),
        1226,
    );

    println!(
        "=> Day 1 part b: The increase count is {} (expected {})",
        count_depth_inc_on3(&depths),
        1252,
    );
}

fn count_depth_inc<T: Ord>(xs: &Vec<T>) -> u32 {
    xs.iter()
        .zip(xs.iter().skip(1))
        .fold(0, |acc, (x, y)| acc + (x < y) as u32)
}

fn count_depth_inc_on3(xs: &[i32]) -> u32 {
    xs.iter()
        .zip(xs.iter().skip(3))
        .fold(0, |acc, (x, y)| acc + (x < y) as u32)
}
