import * as http from "node:http";

async function collectOnRequests(addrs) {
    const dataCollection = await Promise.all(
        addrs.map((addr) => {
            return new Promise((resolve) => {
                http.get(addr, (res) => {
                    let rawData = "";
                    res.on(
                        "data",
                        (newData) => (rawData += newData.toString()),
                    );
                    res.on("error", (err) =>
                        console.error(`Received error: ${err.message}`),
                    );
                    res.on("end", () => resolve(rawData));
                });
            });
        }),
    ).then((data) => data.forEach((res) => console.log(res)));
    return dataCollection;
}

function main() {
    const addrs = process.argv.slice(2);
    collectOnRequests(addrs);
}

main();
