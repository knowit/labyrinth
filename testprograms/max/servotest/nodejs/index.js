const Serialport = require("serialport");
const Board = require("firmata");
const path = require('path');
const Max = require('max-api');

Board.requestPort((error, port) => {
  if (error) {
    console.log(error);
    return;
  }

  const board = new Board(port.comName);

  Max.outlet('initializing');

  board.on("open", () => {
    Max.outlet('open');
    Max.post('open');
  });

  Max.addHandler("mode", (pin, mode) => {
    switch (mode) {
      case 'pwm':
        Max.post(`mode pin ${pin} : pwm`);
        board.pinMode(pin, board.MODES.PWM);
        break;
      case 'digitalin':
        Max.post(`mode pin ${pin} : digital in`);
        board.pinMode(pin, board.MODES.INPUT);
        board.digitalRead(pin, (data) => {
          Max.outlet(`${pin} digitalin ${data}`);
        });
        break;
      case 'digitalout':
        Max.post(`mode pin ${pin} : digital out`);
        board.pinMode(pin, board.MODES.OUTPUT);
        break;
      case 'servo':
        Max.post(`mode pin ${pin} : servo`);
        board.pinMode(pin, board.MODES.SERVO);
        break;
      case 'analogin':
        Max.post(`mode pin ${pin} : analog in`);
        board.pinMode(pin, board.MODES.ANALOG);
        board.analogRead(pin, (data) => {
          Max.outlet(`${pin} analogin ${data}`);
        });
        break;
      default:
    }
  });
  Max.addHandler("set", (mode, pin, state) => {
    switch (mode) {
      case 'digitalout':
        // Max.post(`set pin ${pin} : ${state}`);
        let level =  state === 1 ? board.HIGH :  board.LOW;
        board.digitalWrite(pin, level);
        break;
      case 'servo':
        // Max.post(`set pin ${pin} : ${state}`);
        board.servoWrite(pin, state);
        break;
      default:
    }
  });

  /*
  Max.addHandler("AO", (data1, data2) => {
    board.pinMode(3, board.MODES.PWM);
    board.analogWrite(3, data1)
  });


  var timeout = setTimeout(() => {
    Max.outlet('connecting');
    board.pinMode(7, board.MODES.INPUT);
    board.digitalRead(7, (data) => {
      Max.outlet(`7 DI ${data}`);
    });


    board.pinMode(0, board.MODES.ANALOG);
    board.analogRead(0, (data) => {
      Max.outlet(`0 AI ${data}`);
    });
    Max.outlet('connected');
  }, 4000);

*/


});
