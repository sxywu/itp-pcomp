// const http = require("http");
// const express = require("express");
import fs from "fs";
import csvjson from "csvjson";
import _ from "lodash";
import * as d3 from "d3";

import SerialPort from "serialport";
let myPort;

// const server = express();
// const httpServer = http.createServer(server)

// prepare data
const today = new Date();
let data = fs.readFileSync("data.csv", { encoding: "utf-8" }).split("\r\n");
const keys = data[0].split(",");
data = _.chain(data)
  .tail() // take out the first row which is the header
  .map((day) => {
    // convert data into JSON
    return _.chain(day.split(","))
      .map((value, i) => {
        const key = keys[i];
        return [key, key === "date" ? new Date(value) : !!value];
      })
      .fromPairs()
      .value();
  })
  .value();
console.log(data);

// convert day of week to being Monday start
function convertDay(day) {
  day = day.getDay() - 1;
  return day < 0 ? 6 : day;
}

function sendData() {
  const day = convertDay(today);
  const dayRow = _.times(7, (i) => +(i === day));
  console.log(dayRow);
  const dataRows = _.chain(data)
    .takeRight(day + 1)
    .map(({ lunch, dinner, other }) => [+lunch, +dinner, +other])
    .flatten()
    .value();
  console.log(dataRows);
  // myPort.write();
}

// sendData();

function saveData() {
  // convert back to CSV and save
}

function receiveData(data) {
  console.log(data, data.toString());
  data = data.toString();
  if (data === "ready") {
    // Arduino is ready, send day & data back
    sendData();
  } else {
    // receiving updates in format: date,option,true/false
    const [day, option, boolean] = data.split(",");
  }
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
    myPort = new SerialPort(port.path, { baudRate: 9600 });
    myPort.on("open", () => console.log("port open!"));
    myPort.on("data", receiveData);
  },
  (err) => console.error(err)
);
