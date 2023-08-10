import * as fs from "node:fs";

const INPUT_PATH = "input/day4.txt";
const FIELDS = ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid", "cid"];
type Passport = { [key: string]: string };

function withValidFields(
    passports: Array<Passport>,
    ignore: Array<string> = ["cid"],
): number {
    const fields = FIELDS.reduce(
        (ids: Array<string>, id: string) =>
            ignore.includes(id) ? ids : [...ids, id],
        [],
    );
    return passports.reduce(
        (acc: number, p) =>
            fields.every((f) => Object.hasOwn(p, f)) ? acc + 1 : acc,
        0,
    );
}

function withValidFieldsVals(
    passports: Array<Passport>,
    ignore: Array<string> = ["cid"],
): number {
    const fields = FIELDS.reduce(
        (ids: Array<string>, id: string) =>
            ignore.includes(id) ? ids : [...ids, id],
        [],
    );
    const withFields = passports.filter((p) => fields.every((f) => Object.hasOwn(p, f)));
    return withFields.reduce(
        (acc: number, p) =>
            Object.keys(p).every((k) => checkVal(k, p)) ? acc + 1 : acc
        ,
        0,
    );
}

function checkVal(key: string, p: Passport): boolean {
    const val = p[key];
    switch (key) {
        case "byr":
            const byr = Number(val);
            return val.length == 4 && 1920 <= byr && byr <= 2002;
        case "iyr":
            const iyr = Number(val);
            return val.length == 4 && 2010 <= iyr && iyr <= 2020;
        case "eyr":
            const eyr = Number(val);
            return val.length == 4 && 2020 <= eyr && eyr <= 2030;
        case "hgt":
            const hgl = Number(val.slice(0, -2));
            if (val.includes("cm")) {
                return 150 <= hgl && hgl <= 193;
            } else {
                return 59 <= hgl && hgl <= 76;
            }
        case "hcl":
            const validChars = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"];
            if (val.length == 7 && val.charAt(0) == "#") {
                return val.slice(1).split("").every((char) => validChars.includes(char));
            } else {
                return false;
            }
        case "ecl":
            return ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"].includes(
                val,
            );
        case "pid":
            return val.length == 9;
        default:
            return true;
    }
}

export function run() {
    const passports = fs.readFileSync(INPUT_PATH).toString().split("\n\n").map(
        (passport) => {
            return passport.split(/\s|\n/).reduce((acc, str) => {
                const [key, val] = str.split(":");
                return { ...acc, [key]: val };
            }, {});
        },
    );
    console.log(`Day 4 part 1: ${withValidFields(passports)}`);
    console.log(`Day 4 part 2: ${withValidFieldsVals(passports)}`);
}
