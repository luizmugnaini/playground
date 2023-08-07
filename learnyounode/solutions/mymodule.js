const fs = require("node:fs");
const path = require("node:path");

function dirFilesWithExt(dirpath, extension, callback) {
    fs.readdir(dirpath, (err, data) => {
        if (err) {
            return callback(err);
        }
        if (extension) {
            data = data.filter((file_name) => {
                return path.extname(file_name) === "." + extension;
            });
        }
        callback(null, data);
    });
}

module.exports = dirFilesWithExt;
