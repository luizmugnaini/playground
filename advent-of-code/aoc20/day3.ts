import * as fs from "node:fs";

const INPUT_PATH = "input/day3.txt";
const TREE = "#";

interface Position {
    x: number;
    y: number;
}

interface Slope {
    x: number;
    y: number;
}

const SLOPE: Slope = { x: 3, y: 1 };

type Map = Array<Array<string>>;

function move(pos: Position, slope: Slope): Position {
    return {
        x: pos.x + slope.x,
        y: pos.y + slope.y,
    } as Position;
}

function countTreeInPath(map: Map, slope = SLOPE): number {
    const cols = map[0].length, rows = map.length;
    let pos: Position = { x: 0, y: 0 };
    let accTrees = 0;
    while (pos.y < rows) {
        if (map[pos.y][pos.x % cols] == TREE) {
            accTrees += 1;
        }
        pos = move(pos, slope);
    }
    return accTrees;
}

function tooManySlopes(map: Map, slopes: Array<Slope>): number {
    return slopes.reduce(
        (acc: number, slope) => acc * countTreeInPath(map, slope),
        1,
    );
}

export function run() {
    try {
        const map = fs.readFileSync(INPUT_PATH)
            .toString()
            .split("\n")
            .slice(0, -1)
            .map((line) => line.split(""));
        console.log(`Day 3 part 1: ${countTreeInPath(map)}`);
        const slopes: Array<Slope> = [
            { x: 1, y: 1 },
            { x: 3, y: 1 },
            { x: 5, y: 1 },
            { x: 7, y: 1 },
            { x: 1, y: 2 },
        ];
        console.log(`Day 3 part 2: ${tooManySlopes(map, slopes)}`);
    } catch (err: unknown) {
        console.error(
            `Unable to process file ${INPUT_PATH} due to error: ${err}`,
        );
    }
}
