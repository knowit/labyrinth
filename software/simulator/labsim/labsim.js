console.log('Started');
const Max = require('max-api');
const io = require('socket.io-client');

const socket = io.connect('http://localhost:9090');



Max.addHandler("event", (eventName) => {
  socket.emit('event', { name: eventName });
});
