const Serialport = require("serialport");
const Board = require("firmata");

Board.requestPort((error, port) => {
  if (error) {
    console.log(error);
    return;
  }

  const board = new Board(port.comName);

  board.on("open", () => {
    console.log('open');

    board.sendString("a string")

    board.sysexResponse(commandByte, handler)



  });


});
