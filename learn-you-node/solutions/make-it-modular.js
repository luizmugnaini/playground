const dirFilesWithExt = require("./mymodule.js");

function main() {
    const [path, ext] = process.argv.slice(2);
    dirFilesWithExt(path, ext, (err, data) => {
        if (err) {
            return console.error(err);
        }
        data.forEach((file) => console.log(file));
    });
}

main();
