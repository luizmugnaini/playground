const http = require("node:http");

function httpJsonServer(port) {
    http.createServer((req, res) => {
        if (req.method !== "GET" || req.path != "/api/parsetime") {
            return res.end(
                "The server requires 'GET' requests with path '/api/parsetime'",
            );
        }
    });
}

function main() {
    const port = Number(process.argv[2]);
    httpJsonServer(port);
}

main();
