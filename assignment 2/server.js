// const http = require("http");
// const express = require("express");
import fs from "fs";
import csvjson from "csvjson";
import _ from "lodash";
import * as d3 from "d3";
const formatDate = d3.utcFormat("%Y-%m-%d");

import SerialPort from "serialport";
let myPort;

// const server = express();
// const httpServer = http.createServer(server)

// prepare data
const today = d3.utcDay(new Date());
const todayStr = formatDate(today);

let data = fs.readFileSync("data.csv", { encoding: "utf-8" });
data = csvjson.toSchemaObject(data); // convert to json
const keys = _.keys(data[0]);
// backfill any missing dates
const mostRecent = new Date(_.last(data).date);
_.each(d3.utcDay.range(mostRecent, today), (date) => {
  // create row data
  const d = _.reduce(
    keys,
    (obj, key) => {
      obj[key] = key === "date" ? formatDate(date) : 0;
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

function sendData() {
  // send as a string of 21 0 and 1s
  // where first 7 is the days of week
  // second set of 7 is lunch
  // and last set is dinner
  const monday = d3.utcMonday.floor(today);
  const week = _.filter(data, ({ date }) => new Date(date) >= monday);

  const str = _.chain(keys)
    .map((key) => {
      return _.times(7, (i) => {
        // if in future return 0 for everything
        if (!week[i]) return 0;
        const d = week[i][key];

        if (key === "date") {
          return +(d === todayStr);
        }
        return +d;
      });
    })
    .flatten()
    .value()
    .join(",");

  myPort.write(str);
}

function updateData(option) {
  // get today's data and update it
  option = keys[+option];
  const d = +_.last(data)[option];
  _.last(data)[option] = +!d;

  saveData();
}

function receiveData(data) {
  data = data.toString();
  if (data === "ready") {
    // Arduino is ready, send day & data back
    sendData();
  } else {
    updateData(data);
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
