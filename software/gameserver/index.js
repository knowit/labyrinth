let serverURL = "http://127.0.0.1:9090";

const GameController = require('../gamecontroller/js/index');
const socketIOClient = require("socket.io-client");

console.log( JSON.stringify(process.argv,undefined, 2))

let portName = process.argv[2];

g = new GameController();

const socket = socketIOClient(serverURL);
console.log(`connected to server: ${serverURL}`);

const onXBNO = function (value) {
  socket.emit('xbno', {value});
};

const onXSpeed = function (value) {
  socket.emit('xspeed', {value});
};

const onYBNO = function (value) {
  socket.emit('ybno', {value});
};

const onYSpeed = function (value) {
  socket.emit('yspeed', {value});
};

socket.on("xsetpoint", function (data) {
  console.log(`xsetpoint. data:${JSON.stringify(data)}`);
  g.setXAngle(data.value);
});

socket.on("ysetpoint", function (data) {
  console.log(`ysetpoint. data:${JSON.stringify(data)}`);
  g.setYAngle(data.value);
});

console.log(`Connecting to port: ${portName}`);
g.openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed);

console.log("Started");
