use std::fs;

pub fn run() {
    let boards = fs::read_to_string("aoc21/input/day4.txt").expect("Failed to open file");
    println!(
        "=> Day 4 part a: first winning board score = {} (expected {})",
        fst_winning_score(&boards).unwrap(),
        12796,
    );
    println!(
        "=> Day 4 part b: last winning board score = {} (expected {})",
        lst_winning_score(&boards).unwrap(),
        18063,
    );
}

#[derive(Debug)]
struct Board {
    // Vector containing the rows of the board
    elem: Vec<Vec<(i32, bool)>>,
}

impl Board {
    // Returns a board filled with zeros
    fn new() -> Self {
        Self { elem: Vec::new() }
    }

    // Add new row, specified by `string` to the board
    fn new_row(&mut self, string: &str) {
        self.elem.push(
            string
                .split_whitespace()
                .map(|x| (x.parse::<i32>().unwrap(), false))
                .collect::<Vec<(i32, bool)>>(),
        );
    }

    // Marks the occurrences of `n` into a given board
    fn mark_number(&mut self, n: &i32) {
        for row in self.elem.iter_mut() {
            for i in 0..row.len() {
                if row[i].0 == *n {
                    // mark the element
                    row[i] = (row[i].0, true);
                }
            }
        }
    }
}

struct Game {
    nums: Vec<i32>,
    boards: Vec<Board>,
}

impl Game {
    fn new(game: &str) -> Self {
        let mut g = game.lines();

        let nums = g
            .next()
            .unwrap()
            .split(',')
            .map(|x| x.parse::<i32>().unwrap())
            .collect::<Vec<i32>>();

        g.next();
        let boards: Vec<Board> = g
            .fold((Vec::new(), Board::new()), |(mut bs, mut b), l| {
                if l.is_empty() {
                    // Prepare for the next coming board
                    bs.push(b);
                    (bs, Board::new())
                } else {
                    // Insert values in new line of the last board
                    b.new_row(l);
                    if l == game.lines().last().unwrap() {
                        bs.push(b);
                        (bs, Board::new())
                    } else {
                        (bs, b)
                    }
                }
            })
            .0;

        Self { nums, boards }
    }

    fn play_until_win(&mut self) -> Option<(usize, usize)> {
        for (num_index, &n) in self.nums.iter().enumerate() {
            // For each random number, mark the corresponding values on the
            // boards of the game
            for (index, board) in self.boards.iter_mut().enumerate() {
                for row in board.elem.iter_mut() {
                    for i in 0..row.len() {
                        if row[i].0 == n {
                            // mark the element
                            row[i] = (row[i].0, true);
                        }
                    }
                }
                match check_win(board) {
                    true => return Some((index, num_index)),
                    false => continue,
                }
            }
        }
        // No board won the game
        None
    }

    fn play_until_last_win(&mut self) -> Option<(usize, usize)> {
        // `winners` contains the tuples `(b, i)`, where `b` is the board index
        // and `i` is the random number index for which `b` won the game
        let mut winners: Vec<(usize, usize)> = Vec::new();

        // Play all of the numbers for each of the boards,
        for board_index in 0..self.boards.len() {
            for (num_index, &n) in self.nums.iter().enumerate() {
                self.boards[board_index].mark_number(&n);

                // Check if the board won the game, if so, remove it from
                // the `search` vector and put it into
                // the `winners` vector
                match check_win(&mut self.boards[board_index]) {
                    true => {
                        winners.push((board_index, num_index));
                        break;
                    }
                    false => continue,
                }
            }
        }

        // Retun the winner with the latemost drawn random number
        winners.iter().fold(None, |last, &(b, i)| match last {
            None => Some((b, i)),
            Some((_, il)) => {
                if il < i {
                    Some((b, i))
                } else {
                    last
                }
            }
        })
    }
}

//fn last_winner(win: Vec<(usize, usize)>) -> Option<(usize, usize)> {}

fn check_win(board: &mut Board) -> bool {
    // check each row
    for i in 0..board.elem.len() {
        let count_row = board.elem[i].iter().fold(0, |acc, (_, m)| match m {
            true => acc + 1,
            false => acc,
        });
        if count_row == board.elem[i].len() {
            return true;
        } else {
            continue;
        }
    }

    // check each column
    for j in 0..board.elem[0].len() {
        let mut count_col = 0;
        for i in 0..board.elem.len() {
            if board.elem[i][j].1 {
                count_col += 1;
            } else {
                continue;
            }
        }

        if count_col == board.elem[0].len() {
            return true;
        } else {
            continue;
        }
    }

    // If neither the rows nor the columns are full of marked elements, the
    // board doesn't win
    false
}

fn fst_winning_score(data: &str) -> Option<i32> {
    let mut game = Game::new(data);
    if let Some((index, num_index)) = game.play_until_win() {
        // If there is a winning board, return some winning score
        let last_num = game.nums[num_index];
        let sum_unmarked = game.boards[index].elem.iter().fold(0, |acc_row, row| {
            acc_row
                + row.iter().fold(0, |acc, (x, m)| match m {
                    false => acc + x,
                    true => acc,
                })
        });
        return Some(last_num * sum_unmarked);
    }
    // No winning board found
    None
}

fn lst_winning_score(data: &str) -> Option<i32> {
    let mut game = Game::new(data);
    if let Some((index, num_index)) = game.play_until_last_win() {
        // If there is a winning board, return some winning score
        let last_num = game.nums[num_index];
        let sum_unmarked = game.boards[index].elem.iter().fold(0, |acc_row, row| {
            acc_row
                + row.iter().fold(0, |acc, (x, m)| match m {
                    false => acc + x,
                    true => acc,
                })
        });
        return Some(last_num * sum_unmarked);
    }
    // No winning board found
    None
}
