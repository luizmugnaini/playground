import * as fs from "node:fs";

const INPUT_PATH = "input/day2.txt";

interface PwdPolicy {
    letter: string;
    minRepeat: number;
    maxRepeat: number;
    pwd: string;
}

interface NewPwdPolicy {
    letter: string;
    pos1: number;
    pos2: number;
    pwd: string;
}

function part1(input: Array<PwdPolicy>): number {
    // Unfortunately `reduce` imposes `acc: PwdPolicy` which doesn't make sense.
    return input.reduce((acc: number, p) => {
        const numRepeats = p.pwd.split(p.letter).length - 1;
        const within = p.minRepeat <= numRepeats && numRepeats <= p.maxRepeat;
        return within ? acc + 1 : acc;
    }, 0);
}

function part2(input: Array<NewPwdPolicy>): number {
    return input.reduce((acc: number, p) => {
        let hasPos1 = p.pwd.charAt(p.pos1 - 1) == p.letter;
        let hasPos2 = p.pwd.charAt(p.pos2 - 1) == p.letter;
        return hasPos1 !== hasPos2 ? acc + 1 : acc;
    }, 0);
}

export function run() {
    try {
        const file = fs.readFileSync(INPUT_PATH).toString().split("\n").slice(
            0,
            -1,
        );
        const processStr = (str: string) => {
            const [policy, pwd] = str.split(": ");
            const [repeats, letter] = policy.split(" ");
            const [x, y] = repeats.split("-").map((n) => Number(n));
            return [letter, x, y, pwd];
        };

        const input1 = file.map((str) => {
            const [letter, minRepeat, maxRepeat, pwd] = processStr(str);
            return {
                letter: letter,
                minRepeat: minRepeat,
                maxRepeat: maxRepeat,
                pwd: pwd,
            } as PwdPolicy;
        });
        console.log(`Day 2 part 1: ${part1(input1)}`);

        const input2 = file.map((str) => {
            const [letter, pos1, pos2, pwd] = processStr(str);
            return {
                letter: letter,
                pos1: pos1,
                pos2: pos2,
                pwd: pwd,
            } as NewPwdPolicy;
        });
        console.log(`Day 2 part 2: ${part2(input2)}`);
    } catch (err: unknown) {
        console.error(`Unable to process ${INPUT_PATH} due to error: ${err}`);
    }
}
