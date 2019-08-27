const SerialPort = require('serialport')
const MockBinding = require('@serialport/binding-mock')

function translate(input, inputMinA, inputMaxA, outputMin, outputMax) {
  return outputMin + (outputMax - outputMin) * (input - inputMinA) / (inputMaxA - inputMinA)
}

function joinArrays(a, b) {
  const c = new Int16Array(a.length + b.length);
  c.set(a);
  c.set(b, a.length);
  return c;
}


class GameController {

  printPorts() {
    SerialPort.list(function (err, ports) {
      ports.forEach(function(port) {
        console.log(`Found port:  ${port.comName}`);
      });
    });
  }


  openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed) {

    console.log(`Opening port: ${portName}`);

    const port = new SerialPort(portName, {
      baudRate: 115200
    })

    this.port = port;

    var posX = {val: 0, update: false};
    var posY = {val: 0, update: false};

    let serialBuffer = new Uint8Array();
    this.port.on('readable', function () {
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
          var x = translate(value, 0, 16000, -90, 90)
          onXBNO(x.toFixed(2));
          posX.val = x; posX.update = true;
        }

        if (packet[0] === 2) {
          onXSpeed(translate(value, 0, 16000, -90, 90).toFixed(2));
        }

        if (packet[0] === 3) {
          var y = translate(value, 0, 16000, -90, 90)
          onYBNO(y.toFixed(2));
          posY.val = y; posY.update = true;
        }

        if (packet[0] === 4) {
          onYSpeed(translate(value, 0, 16000, -90, 90).toFixed(2));
        }

        if (posX.update && posY.update) {
          // onPosition({ x: posX.val, y: posY.val })
          posX.update = posY.update = false;
        }

        serialBuffer = serialBuffer.slice(3, 9999);
      }
    })

  }

  msb(numberToSend) {
    return 64 + Math.floor(numberToSend / 192);
  }

  lsb(numberToSend) {
    return 64 + numberToSend % 192;
  }

  write2byteFloat(xPseudoValue) {
    this.port.write([this.msb(xPseudoValue), this.lsb(xPseudoValue)]);
    this.port.drain();
  }

  setAngle(x, y) {
    this.port.write([1]);
    this.write2byteFloat(translate(x + 50, 0, 100, 0, 16000));
    this.port.write([11]);
    this.write2byteFloat(translate(y + 50, 0, 100, 0, 16000));
  }

  setXAngle(x) {
    this.port.write([1]);
    this.write2byteFloat(translate(x + 50, 0, 100, 0, 16000));
  }

  setYAngle(y) {
    this.port.write([11]);
    this.write2byteFloat(translate(y + 50, 0, 100, 0, 16000));
  }

  setXKp(t) {
    this.port.write([2]);
    this.write2byteFloat(translate(t, 0, 100, 0, 16000));
  }

  setYKp(t) {
    this.port.write([12]);
    this.write2byteFloat(translate(t, 0, 100, 0, 16000));
  }

  setXKi(s) {
    this.port.write([3]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

  setYKi(s) {
    this.port.write([13]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

  setXKd(s) {
    this.port.write([4]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

  setYKd(s) {
    this.port.write([14]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

  setXCalibration(s) {
    this.port.write([5]);
    this.write2byteFloat(translate(s + 50, 0, 100, 0, 16000));
  }

  setYCalibration(s) {
    this.port.write([15]);
    this.write2byteFloat(translate(s + 50, 0, 100, 0, 16000));
  }

  /*
  setXKd(s) {
    this.port.write([4]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

  setXMinSpeed(s) {
    this.port.write([5]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }
  */

}


module.exports = GameController;

