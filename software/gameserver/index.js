var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');
const GameController = require('../gamecontroller/js/index');
var gamepad = require("gamepad");

gamecontroller = new GameController();

var score = 0;
var inGame = false;
var serverSocket;

app.listen(9090);

let portName = process.argv[2];


function addGamePadSupport() {
  console.log("init gamepad");
// Initialize the library
  gamepad.init()

// Create a game loop and poll for events
  setInterval(gamepad.processEvents, 16);
// Scan for new gamepads as a slower rate
  setInterval(gamepad.detectDevices, 500);

// Listen for move events on all gamepads
  gamepad.on("move", function (id, axis, value) {
    /*
    console.log("move", {
      id: id,
      axis: axis,
      value: value,
    });
    */
    if (axis === 7) {
      // console.log(value);
      gamecontroller.setXAngle(value * 5);
    }
    if (axis === 8) {
      // console.log(value);
      gamecontroller.setYAngle(value * 5);
    }
  });
}

addGamePadSupport();

console.log("Started");

function emitToServerSocket(topic, value) {
  if (serverSocket) {
    serverSocket.emit(topic, {value});
  }
}
const onXBNO = function (value) {
  // console.log('xbno', {value});
  emitToServerSocket('xbno', value);
};

const onYBNO = function (value) {
  // console.log('ybno', {value});
  emitToServerSocket('ybno', value);
};
const onXSpeed = function (value) {
  // console.log('xspeed', {value});
  emitToServerSocket('xspeed', value);
};

const onYSpeed = function (value) {
  // console.log('yspeed', {value});
  emitToServerSocket('yspeed', value);
};

function handler(req, res) {
  fs.readFile(__dirname + '/index.html',
    function (err, data) {
      if (err) {
        res.writeHead(500);
        return res.end('Error loading index.html');
      }

      res.writeHead(200);
      res.end(data);
    });
}

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
  score = 1000;
  inGame = true;
  emitScore(score);
}

console.log(`Connecting to port: ${portName}`);
gamecontroller.openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed);

io.on('connection', function (socket) {
  console.log(`socket.io event: connection id=${socket.id}`)

  serverSocket = socket;

  socket.on('gamestate', function (data) {
    console.log(`gamestate: ${data.name}`);

    if (data.name === 'gamestarted') {
      gameEventGameStarted();
    }

    if (data.name === 'goal') {
      gameEventGoal();
    }

    if (data.name === 'lost') {
      gameEventLost();
    }

    serverSocket.emit('gamestate', data);
  });


  socket.on('disconnect', function (socket) {
    console.log(`socket.io event: disconnect id=${socket.id}`)
  });

  socket.on('xsetpoint', function (data) {
    // console.log(`${'xsetpoint'} data:${JSON.stringify(data)}`);
    gamecontroller.setXAngle(data.value);
  });

  socket.on('ysetpoint', function (data) {
    // console.log(`${'ysetpoint'} data:${JSON.stringify(data)}`);
    gamecontroller.setYAngle(data.value);
  });


});

function emitScore(t) {
  if (serverSocket) {
    serverSocket.emit('score', {value: t});
    console.log(`emitting score: ${t}`);
  } else {
    console.error("no server socket");
  }
}

setInterval(function () {
  if (inGame) {
    score = score - 10;
    if (score > 0) {
      emitScore(score);
    } else {
      gameEventLost();
    }
  }
}, 500);
