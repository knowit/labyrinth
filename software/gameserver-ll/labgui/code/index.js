const net = require('net')
const protobuf = require("protobufjs");
const path = require('path');
const Max = require('max-api');

// This will be printed directly to the Max console
Max.post(`Loaded the ${path.basename(__filename)} script`);

let rotationx = 0.0;
let rotationy = 0.0;
let positionx = 0.0;
let positiony = 0.0;

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
                //console.log("sending data");

                const message = GameUpdate.fromObject({
                    data: {
                        gameState: {
                            rotation: {
                                x: rotationx,
                                y: rotationy
                            },
                            position: {
                                x: positionx,
                                y: positiony
                            }
                        }
                    }
                })


                const encoded = GameUpdate.encodeDelimited(message).finish();

                socket.write(encoded, null, () => {

                    setTimeout(testWritePos, 20);
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

                // console.log(inputAnalogueAxis, cameraOrientation);
                Max.post(cameraOrientation.w);
                console.log(JSON.stringify(update));
            })
        })
        .catch(err => console.error(err));
})

Max.addHandler("rotationx", (msg) => {
	rotationx = msg;
});

Max.addHandler("rotationy", (msg) => {
    rotationy = msg;
});

Max.addHandler("positionx", (msg) => {
    positionx = msg;
});

Max.addHandler("positiony", (msg) => {
    positiony = msg;
});

server.on('error', err => console.error(err))
server.listen(9091, () => console.log('server bound'))

