const SerialPort = require('serialport')
const port = new SerialPort('/dev/cu.usbmodem142141', {
  baudRate: 115200
})
const Max = require('max-api');

let serialBuffer = new Uint8Array();

function translate(input, inputMinA, inputMaxA, outputMin, outputMax) {
  return outputMin + (outputMax - outputMin) * (input - inputMinA) / (inputMaxA - inputMinA)
}

function joinArrays(a, b) {
  const c = new Int16Array(a.length + b.length);
  c.set(a);
  c.set(b, a.length);
  return c;
}

port.on('readable', function () {
  serialBuffer = joinArrays(serialBuffer, port.read());
  while (serialBuffer.length > 3) {
    while (serialBuffer[0] > 63) {
      serialBuffer = serialBuffer.slice(1, 9999);
      console.log('ignoring');
    }
    const packet = serialBuffer.slice(0, 3);

    const msb = packet[1];
    const lsb = packet[2];
    const value = (msb - 64) * 192 + (lsb - 64);
    // Max.post('value:' + value);

    if (packet[0] === 1) { // xBno
      Max.outlet('xbno ' + translate(value, 0, 16000, -90, 90).toFixed(2));
    }


    serialBuffer = serialBuffer.slice(3, 9999);
  }
})

function msb(numberToSend) {
  return 64 + Math.floor(numberToSend / 192);
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


function setXKp(t) {
  port.write([2]);
  write2byteFloat(translate(t, 0, 100, 0, 16000));
}

function setXKi(s) {
  port.write([3]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

function setXKd(s) {
  port.write([4]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

function setXMinSpeed(s) {
  port.write([5]);
  write2byteFloat(translate(s, 0, 90, 0, 16000));
}

Max.addHandler("setAngle", (x, y) => {
  setAngle(x, y);
});

Max.addHandler("setXKp", (t) => {
  setXKp(t);
});

Max.addHandler("setXKi", (s) => {
  setXKi(s);
});

Max.addHandler("setXKd", (s) => {
  setXKd(s);
});

Max.addHandler("setXMinSpeed", (s) => {
  setXMinSpeed(s);
});
