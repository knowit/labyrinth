const SerialPort = require('serialport')


const port = new SerialPort('/dev/cu.wchusbserial1421140', {
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

    if (packet[0] === 1) {
      Max.outlet('x bno ' + translate(value, 0, 16000, -90, 90).toFixed(2));
    }

    if (packet[0] === 2) {
      Max.outlet('x speed ' + translate(value, 0, 16000, -90, 90).toFixed(2));
    }

    if (packet[0] === 3) {
      Max.outlet('y bno ' + translate(value, 0, 16000, -90, 90).toFixed(2));
    }

    if (packet[0] === 4) {
      Max.outlet('y speed ' + translate(value, 0, 16000, -90, 90).toFixed(2));
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
  port.drain();
}

function setAngle(x, y) {
  port.write([1]);
  write2byteFloat(translate(x + 50, 0, 100, 0, 16000));
  port.write([11]);
  write2byteFloat(translate(y + 50, 0, 100, 0, 16000));
}


function setXKp(t) {
  port.write([2]);
  write2byteFloat(translate(t, 0, 100, 0, 16000));
}

function setYKp(t) {
  port.write([12]);
  write2byteFloat(translate(t, 0, 100, 0, 16000));
}

function setXKi(s) {
  port.write([3]);
  write2byteFloat(translate(s, 0, 100, 0, 16000));
}

function setYKi(s) {
  port.write([13]);
  write2byteFloat(translate(s, 0, 100, 0, 16000));
}

function setXKd(s) {
  port.write([4]);
  write2byteFloat(translate(s, 0, 100, 0, 16000));
}

function setXMinSpeed(s) {
  port.write([5]);
  write2byteFloat(translate(s, 0, 100, 0, 16000));
}


Max.addHandler("setAngle", (x, y) => {
  setAngle(x, y);
});

Max.addHandler("setXKp", (t) => {
  setXKp(t);
});

Max.addHandler("setYKp", (t) => {
  setYKp(t);
});

Max.addHandler("setXKi", (s) => {
  setXKi(s);
});

Max.addHandler("setYKi", (s) => {
  setYKi(s);
});

Max.addHandler("setXKd", (s) => {
  setXKd(s);
});

Max.addHandler("xMinSpeed", (s) => {
  setXMinSpeed(s);
});
