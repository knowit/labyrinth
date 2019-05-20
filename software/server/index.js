var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

var score = 0;
var inGame = false;
var serverSocket;

server.listen(9090);
// WARNING: app.listen(80) will NOT work here!

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

function gameEventLost() {
  console.log('Lost');
  score = 0;
  console.log(`Score: ${score}`)
  inGame = false;
  emitScore(score);
}

function gameEventGoal() {
  console.log('GOAL!');
  console.log(`Score: ${score}`)
  inGame = false;
  emitScore(score);

}

function gameEventGameStarted() {
  console.log('Game started');
  score = 100;
  inGame = true;
  emitScore(score);
}

io.on('connection', function (socket) {
  console.log(`socket.io event: connection id=${socket.id}`)

  serverSocket = socket;

  socket.on('event', function (data) {
    console.log(`event.name: ${data.name}`);

    if (data.name === 'gamestarted') {
      gameEventGameStarted();
    }

    if (data.name === 'goal') {
      gameEventGoal();
    }

    if (data.name === 'lost') {
      gameEventLost();
    }

    socket.broadcast.emit('event', data);
  });

  socket.on('disconnect', function (socket) {
    console.log(`socket.io event: disconnect id=${socket.id}`)
  });
});

function emitScore(t) {
  if (serverSocket) {
    serverSocket.broadcast.emit('score', {value: t});
    console.log(`emitting score: ${t}`);
  } else {
    console.error("no server socket");
  }
}

setInterval(function () {
  if (inGame) {
    score = score - 10;
    if (score > 0) {
      console.log(`score: ${score}`);
      emitScore(score);
    } else {
      gameEventLost();
    }
  }
}, 500);
