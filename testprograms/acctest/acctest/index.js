const SerialPort = require('serialport')
const port = new SerialPort('/dev/cu.usbmodem142141', {
  baudRate: 115200
})
const Max = require('max-api');

function translate(input, inputMinA, inputMaxA, outputMin, outputMax) {
  return outputMin + (outputMax - outputMin) * (input - inputMinA) / (inputMaxA - inputMinA)
}

const Delimiter = require('@serialport/parser-delimiter')

const parser = port.pipe(new Delimiter({delimiter: [0]}))
parser.on('data', (data) => {
  console.log(data)
})


function msb(numberToSend) {
  let msb = 64 + Math.floor(numberToSend / 192);
  return msb;
}

function lsb(numberToSend) {
  return 64 + numberToSend % 192;
}

function write2byteFloat(xPseudoValue) {
  port.write([msb(xPseudoValue), lsb(xPseudoValue)]);
}

function setAngle(x, y) {
  port.write([1]);
  write2byteFloat(translate(x, -90, 90, 0, 16000));
  write2byteFloat(translate(y, -90, 90, 0, 16000));
}


function setXThreshold(t) {
  port.write([2]);
  write2byteFloat(translate(t, 0, 10, 0, 16000));
}

function setXSpeedFactor(s) {
  port.write([3]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

function setXMaxSpeed(s) {
  port.write([4]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

function setXMinSpeed(s) {
  port.write([5]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

Max.addHandler("setAngle", (x, y) => {
  setAngle(x,y);
});

Max.addHandler("setXThreshold", (t) => {
  setXThreshold(t);
});

Max.addHandler("setXSpeedFactor", (s) => {
  setXSpeedFactor(s);
});

Max.addHandler("setXMaxSpeed", (s) => {
  setXMaxSpeed(s);
});

Max.addHandler("setXMinSpeed", (s) => {
  setXMinSpeed(s);
});
