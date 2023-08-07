const http = require("node:http");
const url = require("node:url");

function requestResult(reqURL) {
    const dt = new Date(reqURL.query["iso"]);
    switch (reqURL.pathname) {
        case "/api/parsetime":
            return {
                hour: dt.getHours(),
                minute: dt.getMinutes(),
                second: dt.getSeconds(),
            };
        case "/api/unixtime":
            return { unixtime: dt.getTime() };
        default:
            console.error(`${reqURL.pathname} is not supported.`);
            return null;
    }
}

function httpServerToUpper(port) {
    http.createServer((req, res) => {
        const reqURL = url.parse(req.url, true);
        const result = requestResult(reqURL);
        if (result) {
            res.writeHead(200, { "Content-type": "application/json" }).end(
                JSON.stringify(result),
            );
        } else {
            res.writeHead(404).end();
        }
    }).listen(port);
}

function main() {
    const port = Number(process.argv[2]);
    httpServerToUpper(port);
}

main();
