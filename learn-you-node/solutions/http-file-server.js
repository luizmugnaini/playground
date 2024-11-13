const fs = require("node:fs");
const http = require("node:http");

async function httpFileServer(port, filePath) {
    const fileStream = fs.createReadStream(filePath);
    http.createServer((_, res) => {
        res.writeHead(200, { "content-type": "text/plain" });
        fileStream.pipe(res);
    }).listen(port);
}

function main() {
    const [port, filePath] = process.argv.slice(2);
    httpFileServer(Number(port), filePath);
}

main();
