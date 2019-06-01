const GameController = require('../gamecontroller/js/')
const net = require('net')

const quatToEulerAxis = (q) => {
	const x = Math.atan2(
        2.0 * (q.w * q.x + q.y * q.z),
        1.0 - 2.0 * (q.x * q.x + q.y * q.y));

    const sinp = 2.0 * (q.w * q.y - q.z * q.x);
    const y = Math.abs(sinp) >= 1 ? Math.sign(sinp) * (Math.PI / 2.0) :  Math.asin(sinp)
 
    const z = Math.atan2(
        2.0 * (q.w * q.z + q.x * q.y), 
        1.0 - 2.0 * (q.y * q.y + q.z * q.z));
    
    // Radians
    return { x, y, z }
}

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

    socket.on('data', (buffer) => {
        var recieved = JSON.parse(buffer.toString('ascii'));
        
        var orientation = quatToEulerAxis(recieved.Rotation);

        controller.setXAngle(orientation.x);
        controller.setYAngle(orientation.z);
    })
})
server.on('error', err => console.error(err))
server.listen(9091, () => console.log('server bound'))