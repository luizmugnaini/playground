use std::{collections::HashMap, fs};

pub fn run() {
    let vent_data = fs::read_to_string("aoc21/input/day5.txt").expect("Failed to open file");
    println!(
        "=> Day 5 part a: number of dangerous points = {} (expected {})",
        dangerous_points(&vent_data),
        6564
    );
}

#[derive(Debug)]
struct Vent {
    p1: (i32, i32),
    p2: (i32, i32),
}

impl Vent {
    fn new(p1: (i32, i32), p2: (i32, i32)) -> Self {
        Self { p1, p2 }
    }
}

struct CoverMap {
    cover: HashMap<(i32, i32), u32>,
}

impl CoverMap {
    fn new() -> Self {
        Self {
            cover: HashMap::new(),
        }
    }

    // Adds new points to the `CoverMap`
    fn add_cover(&mut self, point: (i32, i32)) {
        match self.cover.get_mut(&point) {
            Some(n) => *n += 1,
            None => {
                self.cover.insert(point, 1);
            }
        }
    }

    // Returns a vector containing all points covered by the given vent
    fn coverage_from_vent(&mut self, vent: Vent) {
        if vent.p1.0 == vent.p2.0 {
            // Vertical vent
            let (min, max) = min_max(vent.p1.1, vent.p2.1);
            for y in min..=max {
                self.add_cover((vent.p1.0, y));
            }
        } else if vent.p1.1 == vent.p2.1 {
            // Horizontal vent
            let (min, max) = min_max(vent.p1.0, vent.p2.0);
            for x in min..=max {
                self.add_cover((x, vent.p1.1));
            }
        } else {
            let mut x = vent.p1.0;
            let mut y = vent.p1.1;
            let (dx, dy) = displacement(vent.p1, vent.p2);
            while (x, y) != (vent.p2.0 + dx, vent.p2.1 + dy) {
                self.add_cover((x, y));
                x += dx;
                y += dy;
            }
        }
    }

    fn intersections(&self) -> u32 {
        self.cover
            .iter()
            .fold(0, |acc, (_, &v)| if v > 1 { acc + 1 } else { acc })
    }
}

fn displacement((x1, y1): (i32, i32), (x2, y2): (i32, i32)) -> (i32, i32) {
    let cmp = |a, b| if a < b { 1 } else { -1 };
    (cmp(x1, x2), cmp(y1, y2))
}

// Returns the tuple of minimum and maximum out of two elements
fn min_max(x: i32, y: i32) -> (i32, i32) {
    if y >= x {
        (x, y)
    } else {
        (y, x)
    }
}

fn to_vents(data: &str) -> Vec<Vent> {
    let mut vs: Vec<Vent> = Vec::new();
    for v in data.lines() {
        let mut pts = v.split(" -> ");
        let p1: Vec<i32> = pts
            .next()
            .unwrap()
            .split(',')
            .map(|c| c.parse().unwrap())
            .collect();

        let p2: Vec<i32> = pts
            .next()
            .unwrap()
            .split(',')
            .map(|c| c.parse().unwrap())
            .collect();
        vs.push(Vent::new((p1[0], p1[1]), (p2[0], p2[1])));
    }
    vs
}

fn dangerous_points(data: &str) -> u32 {
    let mut map = CoverMap::new();
    for v in to_vents(data) {
        map.coverage_from_vent(v);
    }
    map.intersections()
}
