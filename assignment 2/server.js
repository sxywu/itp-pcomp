// const http = require("http");
// const express = require("express");
const _ = require("lodash");
const SerialPort = require("serialport");

// const server = express();
// const httpServer = http.createServer(server)

function openPort() {
  console.log("port open!");
}

// first find correct port
SerialPort.list().then(
  (ports) => {
    // find port with Arduino manufacturer
    const port = _.find(
      ports,
      ({ manufacturer }) =>
        manufacturer && _.includes(manufacturer.toLowerCase(), "arduino")
    );
    if (!port) return;

    // if port was found, open serial
    const myPort = new SerialPort(port.path, { baudRate: 9600 });
    myPort.on("open", openPort);
  },
  (err) => console.error(err)
);
