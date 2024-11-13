const fs = require("fs/promises");

function filterByFileExt(file, extension) {
    if (extension) {
        // `ext` will include something like ".txt", ".rs", etc.
        const ext = file.slice(-extension.length - 1);
        if (ext[0] === "." && file.slice(-extension.length) === extension) {
            console.log(file);
        }
        return;
    }
    console.log(file);
}

async function ls(path, extension) {
    try {
        const files = await fs.readdir(path, "utf8");
        files.forEach((f) => filterByFileExt(f, extension));
    } catch (err) {
        console.error(err);
    }
}

function main() {
    const [path, extension] = process.argv.slice(2);
    ls(path, extension);
}

main();
