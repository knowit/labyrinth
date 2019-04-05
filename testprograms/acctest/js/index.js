const SerialPort = require('serialport')
const serialport = new SerialPort('/dev/cu.usbmodem142241')








setTimeout(function () {
  console.log('timeout completed');
  serialport.write('ROBOT POWER ON'); console.log('wrote');
}, 5000);


// Switches the port into "flowing mode"
serialport.on('data', function (data) {
  console.log('data - Data:', data.toString());
});
// Read data that is available but keep the stream from entering //"flowing mode"
serialport.on('readable', function () {
  console.log('readable - Data:', serialport.read().toString());
});
