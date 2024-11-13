use std::{cmp::min, fs};

pub fn run() {
    let crab_positions = fs::read_to_string("input/day7.txt").expect("Failed to open file");
    println!(
        "=> Day 7 part a: minimum spent fuel for alignment = {} (expected {})",
        alignment_fuel(&crab_positions),
        337833
    );
    println!(
        "=> Day 7 part b: minimum spent fuel for alignment = {} (expected {})",
        alignment_fuel_inc(&crab_positions),
        96678050
    );
}

fn floor_ceil(x: f32) -> (i32, i32) {
    (f32::floor(x) as i32, f32::ceil(x) as i32)
}

fn positions(data: &str) -> Vec<i32> {
    data[0..(data.len() - 1)]
        .split(',')
        .map(|x| x.parse().unwrap())
        .collect()
}

fn cost_to(xs: &[i32], finish: i32) -> i32 {
    xs.iter().fold(0, |acc, x| {
        let n = i32::abs(x - finish);
        acc + n * (n + 1) / 2
    })
}

fn alignment_fuel(data: &str) -> i32 {
    let mut xs = positions(data);
    xs.sort();

    // The median is the best alignment position
    let median = if xs.len() % 2 != 0 {
        xs[(xs.len() + 1) / 2 - 1]
    } else {
        (xs[xs.len() / 2 - 1] + xs[xs.len() / 2]) / 2
    };

    xs.iter().fold(0, |acc, x| i32::abs(median - x) + acc)
}

fn alignment_fuel_inc(data: &str) -> i32 {
    let xs = positions(data);
    let (mean1, mean2) = floor_ceil(xs.iter().fold(0, |acc, x| acc + x) as f32 / xs.len() as f32);
    min(cost_to(&xs, mean1), cost_to(&xs, mean2))
}
