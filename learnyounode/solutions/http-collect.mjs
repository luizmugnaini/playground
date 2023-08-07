import * as http from "node:http";

function collectOnRequest(addr) {
    http.get(addr, (res) => {
        const rawData = "";
        res.on("data", (newData) => (rawData += newData));
        res.on("error", (err) =>
            console.error(`Recieved an error: ${err.message}`),
        );
        res.on("end", () => {
            console.log(rawData.length);
            console.log(rawData);
        });
    }).on("error", (err) => {
        console.error(
            `Get request for address ${addr} got an error: ${err.message}`,
        );
    });
}

function main() {
    const addr = process.argv[2];
    collectOnRequest(addr);
}

main();
