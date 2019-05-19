const SerialPort = require('serialport')


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

  openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed) {

    console.log(`Opening port: ${portName}`);

    const port = new SerialPort(portName, {
      baudRate: 115200
    })

    this.port = port;

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
          onXBNO(translate(value, 0, 16000, -90, 90).toFixed(2));
        }

        if (packet[0] === 2) {
          onXSpeed(translate(value, 0, 16000, -90, 90).toFixed(2));
        }

        if (packet[0] === 3) {
          onYBNO(translate(value, 0, 16000, -90, 90).toFixed(2));
        }

        if (packet[0] === 4) {
          onYSpeed(translate(value, 0, 16000, -90, 90).toFixed(2));
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

  setXMinSpeed(s) {
    this.port.write([5]);
    this.write2byteFloat(translate(s, 0, 100, 0, 16000));
  }

}


module.exports = GameController;

