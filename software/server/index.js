var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

server.listen(9090);
// WARNING: app.listen(80) will NOT work here!

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', function (socket) {
  socket.emit('news', { hello: 'world' });
  socket.on('test', function (data) {
    console.log(data);
  });
});
