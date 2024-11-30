mod day1;
mod day2;
mod day3;
mod day4;
mod day5;
mod day6;
mod day7;
use std::env;

fn run_all() {
    day1::run();
    day2::run();
    day3::run();
    day4::run();
    day5::run();
    day6::run();
    day7::run();
}

fn run_day(day: &str) {
    match day {
        "1" => day1::run(),
        "2" => day2::run(),
        "3" => day3::run(),
        "4" => day4::run(),
        "5" => day5::run(),
        "6" => day6::run(),
        "7" => day7::run(),
        _ => {
            println!("Solution of day {} isn't currently unavailable", day);
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() == 1 {
        run_all();
    } else {
        for day in &args[1..] {
            run_day(day);
        }
    }
}
