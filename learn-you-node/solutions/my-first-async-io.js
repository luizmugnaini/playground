const fs = require("fs/promises");

async function countLinesAsync(path) {
    if (path === undefined) {
        throw new Error("A file path must be provided");
    }
    try {
        const content = await fs.readFile(path, "utf8");
        // The last line adds a false idea of the existence of a next line,
        // so we ignore it by subtracting the result by 1.
        return content.split("\n").length - 1;
    } catch (err) {
        console.error(err);
    }
}

function main() {
    const path = process.argv[2];
    countLinesAsync(path).then((count) => console.log(count));
}

main();
