const SerialPort = require('serialport')
const port = new SerialPort('/dev/cu.usbmodem142241', {
  baudRate: 9600
})

function translate(input, inputMinA, inputMaxA, outputMin, outputMax) {
  return outputMin + (outputMax - outputMin) * (input - inputMinA) / (inputMaxA - inputMinA)
}

const Delimiter = require('@serialport/parser-delimiter')

const parser = port.pipe(new Delimiter({delimiter: [0]}))
parser.on('data', (data) => {
  console.log(data)
})


function msb(numberToSend) {
  let msb = 64 + Math.floor(numberToSend / 127);
  return msb;
}

function lsb(numberToSend) {
  return 64 + numberToSend % 127;
}

function write2byteFloat(x) {
  let xPseudoValue = translate(x, -90, 90, 0, 16000);
  port.write([msb(xPseudoValue), lsb(xPseudoValue)]);
}

function setAngle(x, y) {
  port.write([0, 1]);
  write2byteFloat(x);
  write2byteFloat(y);
}

setTimeout(function () {

  setAngle(0.1
    , 90);

}, 2000);

