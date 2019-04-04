const Board = require("firmata");

Board.requestPort((error, port) => {
  if (error) {
    console.log(error);
    return;
  }

  //const board = new Board(port.comName);
  const board = new Board('/dev/cu.usbmodem141241');

  board.on("ready", () => {
    const pin = 13;
    let state = 1;

    board.pinMode(pin, board.MODES.OUTPUT);

    setInterval(() => {
      board.digitalWrite(pin, (state ^= 1));
      board.sysexCommand([0x80,255,100])
    }, 500);
  });
});
