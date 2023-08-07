import * as net from "node:net";

function tcpTimeServer(port) {
    net.createServer((socket) => {
        const padding = (data) => {
            let d = data.toString();
            return (d.length == 1 ? "0" : "") + d;
        };
        const date = new Date();
        socket
            .write(
                data.getFullYear() +
                    "-" +
                    padding(date.getMonth() + 1) +
                    "-" +
                    padding(date.getDate()) +
                    " " +
                    padding(date.getHour()) +
                    ":" +
                    padding(date.getMinutes()),
            )
            .end();
    })
        .listen(port)
        .on("data", (data) => console.log(data.toString()))
        .on("error", (err) =>
            console.error(`Server got an error: ${err.message}`),
        );
}

function main() {
    const port = process.argv[2];
    tcpTimeServer(port);
}

main();
