const SerialPort = require('serialport')
const Readline = require('@serialport/parser-readline')
const maxAPI = require("max-api");

SerialPort.list(function (err, ports) {
  ports.forEach(function(port) {
    console.log(`Found port:  ${port.comName}`);
  });
});

const port = new SerialPort('/dev/tty.SLAB_USBtoUART', {
  baudRate: 115200
})


const parser = port.pipe(new Readline({ delimiter: '\r\n' }))
// parser.on('data', console.log)
parser.on('data', maxAPI.outlet)
