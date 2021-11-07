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
let data = fs.readFileSync("data.csv", { encoding: "utf-8" });
data = csvjson.toSchemaObject(data); // convert to json
const keys = _.keys(data[0]);
// backfill any missing dates
const mostRecent = new Date(_.last(data).date);
_.each(d3.timeDay.range(mostRecent, today), (date) => {
  // create row data
  const d = _.reduce(
    keys,
    (obj, key) => {
      obj[key] = key === "date" ? d3.timeFormat("%Y-%m-%d")(date) : 0;
      return obj;
    },
    {}
  );
  // push in row
  data.push(d);
});

// and then save
function saveData() {
  // convert back to CSV and save
  const csvData = csvjson.toCSV(data, { headers: "key" });
  fs.writeFileSync("data.csv", csvData, { encoding: "utf-8" });
}
saveData();

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
