use std::fs;

pub fn run() {
    let report_a = fs::read_to_string("aoc21/input/day3.txt").expect("Failed to open file");
    println!(
        "=> Day 3 part a: diagnostic report yielded gamma * epsilon = {} (expected {})",
        power_consumption(&report_a),
        3429254
    );

    let report_b = fs::read_to_string("aoc21/input/day3.txt").expect("Failed to open file");
    println!(
        "=> Day 3 part b: o2 * co2 = {} (expected {})",
        life_support_rating(&report_b),
        5410338
    );
}

/// Converts array-like of `bits` into an u32
fn convert(bits: &[u8]) -> u32 {
    bits.iter()
        .fold(0u32, |result, &bit| (result << 1) ^ bit as u32)
}

/// Given a `report`, returns a vector with the frequencies of bits.
/// -> 1 has a positive weight;
/// -> 0 has a negative weight;
fn bit_freq(report: &str) -> Vec<i32> {
    let len_bits = report.lines().next().unwrap().len();
    let conv = |c: Option<char>| match c {
        Some('1') => 1,
        Some('0') => -1,
        _ => panic!("Not a bit"),
    };

    report.lines().fold(vec![0; len_bits], |v, st| {
        let mut s = st.chars();
        v.iter().map(|e| e + conv(s.next())).collect()
    })
}

fn bit_freq_at(s: &str, index: usize) -> i32 {
    let conv = |c: Option<char>| match c {
        Some('1') => 1,
        Some('0') => -1,
        _ => panic!("Not a bit"),
    };

    s.lines()
        .fold(0, |acc, st| acc + conv(st.chars().nth(index)))
}

enum Mode {
    Common,
    Uncommon,
}

fn to_bit(v: &[i32], mode: Mode) -> Vec<u8> {
    let m = match mode {
        Mode::Common => 1,
        Mode::Uncommon => -1,
    };

    v.iter()
        .map(|b| match b {
            t if m * t <= 0 => 0u8,
            _ => 1u8,
        })
        .collect()
}

// Day 3 part A
fn power_consumption(report: &str) -> u32 {
    let freq: Vec<i32> = bit_freq(report);
    let gamma = convert(&to_bit(&freq, Mode::Common));
    let epsilon = convert(&to_bit(&freq, Mode::Uncommon));
    gamma * epsilon
}

fn check_condition(f: i32, b: u8, mode: &Mode) -> bool {
    if let Mode::Common = mode {
        match (f, b) {
            (f, b) if f > 0 && b == 1 => true,
            (f, b) if f < 0 && b == 0 => true,
            (f, b) if f == 0 && b == 1 => true,
            _ => false,
        }
    } else {
        match (f, b) {
            (f, b) if f > 0 && b == 0 => true,
            (f, b) if f < 0 && b == 1 => true,
            (f, b) if f == 0 && b == 0 => true,
            _ => false,
        }
    }
}

fn filter_bits<'a>(state: Vec<&'a str>, index: usize, mode: Mode) -> &'a str {
    if state.len() == 1 {
        state[0]
    } else {
        // Calculate the frequencies for the current state
        let freq_at = bit_freq_at(
            &state.iter().fold(String::new(), |mut acc, s| {
                acc.push_str(s);
                acc.push('\n');
                acc
            }),
            index,
        );

        filter_bits(
            state
                .iter()
                .filter(|line| match line.chars().nth(index).unwrap() {
                    '1' => check_condition(freq_at, 1, &mode),
                    '0' => check_condition(freq_at, 0, &mode),
                    _ => panic!("Not a bit at line {}", line),
                })
                .map(|x| x as &str)
                .collect(),
            index + 1,
            mode,
        )
    }
}

/// Day 3 part B
fn life_support_rating(report: &str) -> u32 {
    let from_str = |s: &str| {
        s.chars()
            .map(|c| c.to_digit(2).unwrap() as u8)
            .collect::<Vec<u8>>()
    };

    let o2 = convert(&from_str(filter_bits(
        report.lines().collect::<Vec<&str>>(),
        0,
        Mode::Common,
    )));
    let co2 = convert(&from_str(filter_bits(
        report.lines().collect::<Vec<&str>>(),
        0,
        Mode::Uncommon,
    )));

    o2 * co2
}
