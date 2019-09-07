const net = require('net')
const protobuf = require("protobufjs");

const getMessages = () => {
    return new Promise((resolve, reject) => {
        protobuf.load("GameUpdate.proto", function(err, root) {
            if (err) return reject(err);

            return resolve({
                GameUpdate: root.lookupType("GameUpdate"),
            })
        });
    })
}

const server = net.createServer(socket => {

    getMessages()
        .then(({ GameUpdate }) => {
            const testWritePos = () => {
                console.log("sending data");
                
                const message = GameUpdate.fromObject({
                    data: {
                        gameState: {
                            rotation: {
                                x: (Math.random() * 4.0) - 2.0,
                                y: (Math.random() * 4.0) - 2.0
                            },
                            position: {
                                x: (Math.random() * 2.0) - 1.0,
                                y: (Math.random() * 2.0) - 1.0
                            }
                        }
                    }
                })
                
                const encoded = GameUpdate.encodeDelimited(message).finish();

                socket.write(encoded, null, () => {
                    setTimeout(testWritePos, 5000);
                });   
            }
            setTimeout(testWritePos, 5000);
        
            socket.on('data', (buffer) => {
                var update = GameUpdate.decodeDelimited(buffer);

                const { 
                    data: { 
                        inputUpdate: { 
                            inputAnalogueAxis, cameraOrientation
                        } 
                    } 
                } = update;

                console.log(inputAnalogueAxis, cameraOrientation);
            })
        })
        .catch(err => console.error(err));
})
server.on('error', err => console.error(err))
server.listen(9091, () => console.log('server bound'))