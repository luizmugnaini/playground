import * as day1 from "./day1.js";
import * as day2 from "./day2.js";
import * as day3 from "./day3.js";
import * as day4 from "./day4.js";

// TODO: This does what it intends to do, however, we need to deal with user authentication yet
// to be able to truly get the input.
/*
import * as nodeFs from "node:fs";
import * as nodeHttps from "node:https";

const INPUT_DIR = "input";
const AOC_URL = "https://adventofcode.com/2020/day";

function downloadInput(day: number) {
    if (day > 25 || day < 1) {
        console.error(`There exists no AOC challenge for day ${day}.`);
    }
    const inputFileExists: boolean = nodeFs.readdirSync(INPUT_DIR).includes("day" + day);
    if (!inputFileExists) {
        const fileStream = nodeFs.createWriteStream(INPUT_DIR + "/day" + day);
        const url = AOC_URL + `/${day}/input`;
        nodeHttps.get(url, (res) =>
            res.pipe(fileStream)
        ).on("error", (err) =>
            console.error(`Unable to get input content from ${url} due to error: ${err.message}`)
        );
    }
}
*/

function runDay(day: number) {
    switch (day) {
        case 1:
            day1.run();
            break;
        case 2:
            day2.run();
            break;
        case 3:
            day3.run();
            break;
        case 4:
            day4.run();
            break;
        default:
            console.log(`Day ${day} is currently unavailable`);
    }
}

function main() {
    process.argv.slice(2).forEach((day) => runDay(Number(day)));
}

main();
