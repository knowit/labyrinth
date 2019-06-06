var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);

var fs = require('fs');
const GameController = require('../gamecontroller/js/index');
var gamepad = require("gamepad");

gamecontroller = new GameController();

var score = 0;
var inGame = false;
var gamestate = 'unknown';
var serverSocket;

server.listen(8080);

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
      startIfPending();
      gamecontroller.setXAngle(value * 5);
    }
    if (axis === 8) {
      // console.log(value);
      startIfPending();
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

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});

app.get('/gamestarted', function (req, res) {
  gameEventGameStarted();
  res.sendFile(__dirname + '/index.html');
});

app.get('/gamelost', function (req, res) {
  gameEventLost();
  res.sendFile(__dirname + '/index.html');
});

app.get('/gamegoal', function (req, res) {
  gameEventGoal();
  res.sendFile(__dirname + '/index.html');
});

app.get('/gamepending', function (req, res) {
  gameEventGamePending();
  res.sendFile(__dirname + '/index.html');
});


function gameEventLost() {
  gamestate = "gamelost";
  serverSocket.emit('gamestate', {name: 'gamelost'});
  console.log('Lost');
  score = 0;
  console.log(`Score: ${score}`)
  inGame = false;
  emitScore(score);
  gameEventGamePending();
}

function gameEventGoal() {
  gamestate = "gamegoal";
  serverSocket.emit('gamestate', {name: 'gamegoal'});
  console.log('GOAL!');
  console.log(`Score: ${score}`)
  inGame = false;
  emitScore(score);
  gameEventGamePending();
}

function gameEventGameStarted() {
  serverSocket.emit('gamestate', {name: 'gamestarted'});
  gamestate = "gamestarted";
  console.log('Game started');
  score = 1000;
  inGame = true;
  emitScore(score);
}

function gameEventGamePending() {
  gamestate = "gamepending";
  serverSocket.emit('gamestate', {name: 'gamepending'});
  console.log('Game pending');
  score = 0;
  inGame = false;
  gamecontroller.setXAngle(-2);
  gamecontroller.setYAngle(2);
  emitScore(score);
}

console.log(`Connecting to port: ${portName}`);
gamecontroller.openPort(portName, onXBNO, onXSpeed, onYBNO, onYSpeed);

function startIfPending() {
  console.log(`startIfPending, gamestate=${gamestate}`);
  if (gamestate === 'gamepending') {
    gameEventGameStarted();
  }
}

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
    startIfPending();
    gamecontroller.setXAngle(data.value);
  });

  socket.on('ysetpoint', function (data) {
    // console.log(`${'ysetpoint'} data:${JSON.stringify(data)}`);
    startIfPending();
    gamecontroller.setYAngle(data.value);
  });

  socket.on('xKp', function (data) {
    console.log(`${'xKp'} data:${JSON.stringify(data)}`);
    gamecontroller.setXKp(data.value);
  });

  socket.on('yKp', function (data) {
    console.log(`${'yKp'} data:${JSON.stringify(data)}`);
    gamecontroller.setYKp(data.value);
  });

  socket.on('xKi', function (data) {
    console.log(`${'xKi'} data:${JSON.stringify(data)}`);
    gamecontroller.setXKi(data.value);
  });

  socket.on('yKi', function (data) {
    console.log(`${'yKi'} data:${JSON.stringify(data)}`);
    gamecontroller.setYKi(data.value);
  });

  socket.on('xKd', function (data) {
    console.log(`${'xKd'} data:${JSON.stringify(data)}`);
    gamecontroller.setXKd(data.value);
  });

  socket.on('yKd', function (data) {
    console.log(`${'xKd'} data:${JSON.stringify(data)}`);
    gamecontroller.setYKd(data.value);
  });

  socket.on('xCalibration', function (data) {
    console.log(`${'xCalibration'} data:${JSON.stringify(data)}`);
    gamecontroller.setXCalibration(data.value);
  });

  socket.on('yCalibration', function (data) {
    console.log(`${'yCalibration'} data:${JSON.stringify(data)}`);
    gamecontroller.setYCalibration(data.value);
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


var hasPerformedInit = false;
setInterval(function () {
  if (!hasPerformedInit) {
    gameEventGamePending();
    hasPerformedInit = true;
  }
}, 3000);
