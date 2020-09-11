const GameController = require('../../../js/index');
const Max = require('max-api');

console.log( JSON.stringify(process.argv,undefined, 2))

let portName = process.argv[2];

g = new GameController();

const onXBNO = function (value) {
  Max.outlet('x bno ' + value);
};

const onXSpeed = function (value) {
  Max.outlet('x speed ' + value);
};

const onYBNO = function (value) {
  Max.outlet('y bno ' + value);
};

const onYSpeed = function (value) {
  Max.outlet('y speed ' + value);
};

g.openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed);

Max.addHandler("setAngle", (x, y) => {
  g.setAngle(x, y);
});

Max.addHandler("setXKp", (t) => {
  g.setXKp(t);
});

Max.addHandler("setYKp", (t) => {
  g.setYKp(t);
});

Max.addHandler("setXKi", (s) => {
  g.setXKi(s);
});

Max.addHandler("setYKi", (s) => {
  g.setYKi(s);
});

Max.addHandler("setXKd", (s) => {
  g.setXKd(s);
});

Max.addHandler("xMinSpeed", (s) => {
  g.setXMinSpeed(s);
});
