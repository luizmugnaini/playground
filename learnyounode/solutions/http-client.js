const http = require("node:http");

function getRequestData(addr) {
    http.get(addr, (res) => {
        res.setEncoding("utf8");
        let incomingData = "";
        res.on("data", (newData) => {
            incomingData += newData + "\n";
        });
        res.on("error", (err) => console.error(`Recieved an error: ${err}`));
        res.on("end", () => console.log(incomingData));
    }).on("error", (err) => {
        console.error(
            `Get request for address ${addr} failed with error: ${err}`,
        );
    });
}

function main() {
    const addr = process.argv[2];
    getRequestData(addr);
}

main();
