use std::{collections::VecDeque, fs};

pub fn run() {
    let fish_timer = fs::read_to_string("input/day6.txt").expect("Failed to open file");
    println!(
        "=> Day 6 part a: number of lanternfishes after 80 days = {} (expected {})",
        evolution_model(&fish_timer, 80),
        353079
    );
    println!(
        "=> Day 6 part b: number of lanternfishes after 256 days = {} (expected {})",
        evolution_model(&fish_timer, 256),
        1605400130036
    );
}

struct LanternfishSchool {
    states_count: VecDeque<usize>,
}

impl LanternfishSchool {
    fn from(states_count: VecDeque<usize>) -> Self {
        Self { states_count }
    }

    fn next_day(&mut self) {
        // Next day each timer decreases by one
        self.states_count.rotate_left(1);
        self.states_count[6] += self.states_count[8];
    }

    fn count(&self) -> usize {
        self.states_count.iter().fold(0, |acc, x| acc + x)
    }
}

fn data_to_count(data: &str) -> VecDeque<usize> {
    let mut states_count: VecDeque<usize> = vec![0; 9].into();
    let input: Vec<usize> = data
        .chars()
        .filter(|&c| c != ',' && c != '\n')
        .map(|t| t.to_digit(10).unwrap() as usize)
        .collect();
    for state in input {
        states_count[state] += 1;
    }
    states_count
}

fn evolution_model(data: &str, day: u32) -> usize {
    // First day
    let mut school = LanternfishSchool::from(data_to_count(data));

    // Simulation model
    for _ in 0..day {
        school.next_day();
    }
    school.count()
}
