# radiohead-serial advanced usage

This Readme contains information about the advanced usage of the `radiohead-serial` Node.js module.

For the general usage see [README.md](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/README.md).


## Exported classes

### RadioHeadSerial
The RadioHeasSerial main class for sending and receiving messages through the RadioHead network.

For more information see [README.md](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/README.md).

### RH_Serial(port, baud)
Driver to send and receive unaddressed, unreliable datagrams via a serial connection.

For more information see [src/RH_Serial.ts](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/src/RH_Serial.ts).


### RHDatagram
Manager class for addressed, unreliable messages.

For more information see [src/RHDatagram.ts](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/src/RHDatagram.ts).

### RHReliableDatagram
RHDatagram subclass for sending addressed, acknowledged, retransmitted datagrams.

For more information see [src/RHReliableDatagram.ts](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/src/RHReliableDatagram.ts).


## Exported Constants

#### version
The actual version of the module.

### RH_SERIAL_MAX_MESSAGE_LEN = 60
The maximum supported message length.
This is the maximum size for a Buffer used for sending or receiving messages.

### RH_SERIAL_MAX_PAYLOAD_LEN = 64
Maximum message length (including the headers) we are willing to support.

### RH_SERIAL_HEADER_LEN = 4
The length of the headers we add.
The headers are inside the payload and are therefore protected by the FCS.

### RH_FLAGS_NONE = 0
No header Flags.

### RH_FLAGS_RESERVED = 0xf0
Header flags reserved for RadioHead.

### RH_FLAGS_APPLICATION_SPECIFIC = 0x0f
Header flags for the application.
These can be used to transport custom flags.

### RH_FLAGS_ACK = 0x80
Header flags for an ACK message.

### RH_BROADCAST_ADDRESS = 0xff
This is the address that indicates a broadcast.

### RH_DEFAULT_TIMEOUT = 200
The default retry timeout in milliseconds for `RHReliableDatagram`.

### RH_DEFAULT_RETRIES = 3
The default number of retries for `RHReliableDatagram`.


## Exported TypeScript interfaces

### RH_ReceivedMessage
Interface for a received message containing:
* `data:Buffer` - Buffer containing the received data.
* `length:number` - Length of the received data.
* `headerTo:number` - TO header.
* `headerFrom:number` - FROM header.
* `headerId:number` - ID header.
* `headerFlags:number` - FLAGS header.
