import * as fs from "node:fs";

const INPUT_PATH = "input/day1.txt";

function part1(input: Array<number>): number | undefined {
    for (let i = 0; i < input.length; i++) {
        const val = input.find((elem, j) => elem + input[i] == 2020 && j !== i);
        if (val) {
            return input[i] * val;
        }
    }
}

function part2(input: Array<number>): number | undefined {
    for (let i = 0; i < input.length; i++) {
        for (let j = 0; j < input.length; j++) {
            if (j !== i) {
                const sumTwo = input[i] + input[j];
                if (sumTwo <= 2020) {
                    const val = input.find((elem, k) =>
                        elem + sumTwo == 2020 && (k !== i || k !== j)
                    );
                    if (val) {
                        return input[i] * input[j] * val;
                    }
                }
            }
        }
    }
}

export function run() {
    try {
        const input = fs.readFileSync(INPUT_PATH)
            .toString()
            .split("\n")
            .map((x) => parseInt(x, 10));
        console.log(`Day 1 part 1: ${part1(input)}`);
        console.log(`Day 1 part 2: ${part2(input)}`);
    } catch (err: unknown) {
        console.error(
            `Could not process input file ${INPUT_PATH} due to error: ${err}`,
        );
    }
}
