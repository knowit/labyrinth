const GameController = require('../gamecontroller/js/')
const net = require('net')

const controller = new GameController()
const portName = process.argv[2]

const server = net.createServer(socket => {

    console.log(`Connecting to port: ${portName}`);
    controller
        .openPort(
            portName, 
            value => {
                // onXBNO
            }, 
            value => {
                // onXSpeed
            },
            value => {
                // onYBNO
            }, 
            value => {
                // onYSpeed
            });

    socket.on('data', (data) => {
        
        // controller.setXAngle(value * 5);
        // controller.setYAngle(value * 5);
    })
})
server.on('error', err => console.error(err))
server.listen(9091, () => console.log('server bound'))