const net = require('net')

const server = net.createServer(socket => {

    const testWritePos = () => {
        console.log("sending data");
        
        // [ xpos, ypos, xrot, yrot ]
        var data = new Float32Array([
            (Math.random()*2.0)-1.0, (Math.random()*2.0)-1.0, 
            (Math.random() * 4.0) - 2.0, (Math.random() * 4.0) - 2.0
        ]);
        socket.write(Buffer.from(data.buffer));

        setTimeout(testWritePos, 100);
    }
    setTimeout(testWritePos, 100);

    socket.on('data', (buffer) => {

        // X axis [1,-1] and Y Axis [1,-1] 
        var floats = [buffer.readFloatLE(0), buffer.readFloatLE(4)];
        console.log(floats);
    })
})
server.on('error', err => console.error(err))
server.listen(9091, () => console.log('server bound'))