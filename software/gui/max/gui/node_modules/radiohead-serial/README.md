# radiohead-serial

[![build status](https://git.cryhost.de/crycode/node-radiohead-serial/badges/master/build.svg)](https://git.cryhost.de/crycode/node-radiohead-serial/commits/master)
[![coverage report](https://git.cryhost.de/crycode/node-radiohead-serial/badges/master/coverage.svg)](https://git.cryhost.de/crycode/node-radiohead-serial/commits/master)
[![npm version](https://badge.fury.io/js/radiohead-serial.svg)](https://badge.fury.io/js/radiohead-serial)

Communication between some **RadioHead** nodes and **Node.js** using the *RH_Serial* driver and the *RHReliableDatagram* or *RHDatagram* manager of the RadioHead library.

With `radiohead-serial` you can build reliable networks based on serial hardware (e.g. RS485 or RS232) between multiple different devices like regular computers, minicomputers (e.g. Raspberry Pi) and microprocessors (e.g. Arduino). It is also possible to include radio hardware using a microprocessor (e.g. an Arduino nano) as a serial-radio gateway.

[RadioHead](http://www.airspayce.com/mikem/arduino/RadioHead/) is a Packet Radio library for embedded microprocessors.
> RH_Serial works with RS232, RS422, RS485, RS488 and other point-to-point and multidropped serial connections, or with TTL serial UARTs such as those on Arduino and many other processors, or with data radios with a serial port interface. RH_Serial provides packetization and error detection over any hardware or virtual serial connection.

> RHReliableDatagram provides Addressed, reliable, retransmitted, acknowledged variable length messages.

Based on RadioHead library *1.74 2017-03-08*

Since Version 3 this module is based on a TypeScript/JavaScript port of the native RadioHead library.

This module can be used on any Linux, Mac or Windows system, for example a Raspberry Pi with Raspbian or a regular computer.

**Hint:** Version 4 of `radiohead-serial` requires Node.js version 8 or higher. For older Node.js releases you may use the v3 branch of `radiohead-serial` which supports Node.js version 4 or higher.


## Example scenario for using radiohead-serial

The radiohead-serial module is perfect if you want to build your own bus system based on for example RS485.

As a head station you can use a Raspberry Pi minicomputer with a USB-RS485 adapter.
The other nodes on the bus can be some microprocessors (e.g. ATMega8 or Arduino) with an TTL-RS485 converter (e.g. Max485) connected.
In addition using a serial-radio gateway is possible (see below).


## Using other RadioHead drivers with a serial-radio gateway

If you want to use other RadioHead drivers (for example *RH_ASK*), you can simply use an Arduino nano ($2 to $10) as an serial gateway.
Other microprocessors can be used too.

Connect your radio hardware to the Arduino and upload the `rh_serial_ask_gateway` sketch. Some example sketches are included in the [*examples*](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/examples) directory.
The Arduino will act as a gateway between the serial and the radio network.

Optionally the gateway can filter messages, so that only a specific address range is transmitted through the radio network.


## Installation

To install the latest version simply use:
```
npm install radiohead-serial
```

If you want install the latest version of the *v3* branch use:
```
npm install radiohead-serial@3
```


## Examples

The examples blow can be found in the [*examples*](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/examples) directory of this package together with TypeScript examples and a gateway Arduino sketch.

The examples assume a Linux system with two USB-RS485 adapters connected.
The A and B lines of the RS485 are connected between both adapters.
You can also use two machines with respectively one adapter.

Depending on your system you may have to change the used ports (/dev/ttyUSB0) in the examples.

If you want to use ES6 style imports you can use
```ts
import {RadioHeadSerial} from 'radiohead-serial';
```

### A server replying to messages sent by clients
```js
// Require the radiohead-serial module
var RadioHeadSerial = require('radiohead-serial').RadioHeadSerial;

// Create an instance of the RadioHeadSerial class
var rhs = new RadioHeadSerial('/dev/ttyUSB0', 9600, 0x01);
//var rhs = new RadioHeadSerial('COM1', 9600, 0x01);

// Listen to the 'data' event for received messages
rhs.on('data', function(message){
  // Print the received message object
  console.log('-> recv:', message);

  // Convert the decimal from address to hex
  var sender = ('0' + message.headerFrom.toString(16)).slice(-2).toUpperCase();

  // Print a readable form of the data
  if(message.length > 0){
    console.log('-> received ' + message.length + ' bytes from 0x' + sender + ': "' + message.data.toString() + '"');
  }

  // Create the answer for the client
  var answer = new Buffer('Hello back to you, client!');

  // Send the answer to the client
  rhs.send(message.headerFrom, answer).then(function(){
    // Message has been sent successfully
    console.log('<- sent to 0x' + sender + ': "' + answer.toString() + '" Raw:', answer);
  }).catch(function(error){
    // Error while sending the message
    console.log('<- ERROR', error);
  });
});

// Print some info
console.log('Server example running.');
console.log('Now start the client example...');
```

### A client sending messages to a server
```js
// Require the radiohead-serial module
var RadioHeadSerial = require('radiohead-serial').RadioHeadSerial;

// Create an instance of the RadioHeadSerial class
var rhs = new RadioHeadSerial('/dev/ttyUSB1', 9600, 0x02);
//var rhs = new RadioHeadSerial('COM2', 9600, 0x02);

// Listen on the 'data' event for received messages
rhs.on('data', function(message){
  // Print the received message object
  console.log('-> recv:', message);

  // Convert the decimal from address to hex
  var sender = ('0' + message.headerFrom.toString(16)).slice(-2).toUpperCase();

  // Print a readable form of the data
  if(message.length > 0){
    console.log('-> received ' + message.length + ' bytes from 0x' + sender + ': "' + message.data.toString() + '"');
  }
});

// Counter for the number of send messages
var sentCount = 0;

// Function to send a message (calls itself with a timeout until five messages are sent)
function sendData(){
  // Create the data to be send to the server
  var data = new Buffer('Hello server!');

  // Send the data to the server
  rhs.send(0x01, data).then(function(){
    // Message has been sent successfully
    console.log('<- sent to 0x01: "' + data.toString() + '" Raw:', data);

  }).catch(function(error){
    // Error while sending the message
    console.log('<- ERROR', error);

  }).finally(function(){
    // After sending the message, even if failed
    sentCount++;

    // 5 times sent?
    if(sentCount < 5){
      // Send a new message after 2 seconds
      setTimeout(sendData, 2000);
    }else{
      // Close the SerialPort worker after 1 second and exit the client example
      // Use the timeout before close() to receive the answer from the server
      setTimeout(function(){
        rhs.close().then(function(){
          // The SerialPort is now closed
          console.log('Client example done. :-)');
        });
      }, 1000);
    }
  });
}

// Trigger sending the first message
sendData();

// Print some info
console.log('Client example running.');
console.log('I\'ll try to send hello to the Server five times...');
```


## APIv3

*The new APIv3 uses __Events__ and __Promises__ and has some breaking changes against the old APIv1.*

Receiving and sending of messages is always done asynchronous.

TypeScript typings are included in the package.


### RadioHeadSerial(port, baud, address, reliable)
```ts
constructor(port:string, baud:number, address:number, reliable:boolean=true);
```
Constructor of the RadioHeadSerial class.
Loads and initializes the RadioHead driver and manager.

* `port` - The serial port/device to be used for the communication. For example /dev/ttyUSB0 or COM1.
* `baud` - The baud rate to be used for the communication. Supported are 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400.
* `address` - The address of this node in the RadioHead network. Address range goes from 1 to 254.
* `reliable` - (optional) false if RHDatagram should be used instead of RHReliableDatagram. (default true)

### rhs.close()
```ts
close():Promise<{}>;
```
Closes the serial port.
After close() is called, no messages can be received.
Returns a promise which will be resolved if the serial port is closed.

### rhs.send(to, data, length)
```ts
send(to:number, data:Buffer, length?:number):Promise<{}>;
```
Sends a message through the RadioHead network.
Returns a Promise which will be resolved when the message has been sent, or rejected in case of an error.

* `to` - Recipient address. Use 255 for broadcast messages.
* `data` - Buffer containing the message to send.
* `length` - *Optional* number of bytes to send from the buffer. If not given the whole buffer is sent. The maximum length is 60 bytes.

### rhs.setAddress(address)
```ts
setAddress(address:number):void;
```
Sets the address of this node in the RadioHead network.

* `address` - The new address. Address range goes from 1 to 254.

### rhs.thisAddress()
```ts
thisAddress():number;
```
Returns the address of this node.

### rhs.setRetries(count)
```ts
setRetries(count:number):void;
```
Sets the maximum number of retries.
Defaults to 3 at construction time.
If set to 0, each message will only ever be sent once.

* `count` - New number of retries.

### rhs.getRetries()
```ts
getRetries():number
```
Returns the currently configured maximum retries count.

### rhs.setTimeout(timeout)
```ts
setTimeout(timeout:number):void;
```
Sets the minimum retransmit timeout in milliseconds.
If an ack is taking longer than this time, a message will be retransmitted.
Default is 200.

* `timeout` - New timeout in milliseconds.

### rhs.getRetransmissions()
```ts
getRetransmissions():number;
```
Returns the number of retransmissions we have had to send since starting or since the last call to resetRetransmissions().

### rhs.resetRetransmissions()
```ts
resetRetransmissions():void;
```
Resets the count of the number of retransmissions to 0.

### rhs.setPromiscuous(promiscuous)
```ts
setPromiscuous(promiscuous:boolean):void;
```
Tells the receiver to accept messages with any to address, not just messages addressed to this node or the broadcast address.

* `promiscuous` - true if you wish to receive messages with any to address. (default false)

### rhs.on('data', function(receivedData){ })
```ts
rhs.on('data', (message:RH_ReceivedMessage) => { /* do something */ });
```
The `data` event is emitted for every received message and includes an object with the following information.

* `data` - The received data as a Buffer.
* `length` - The length of the received data.
* `headerFrom` - The from address of the received message.
* `headerTo` - The to address of the received message.
* `headerId` - The id of the received message.
* `headerFlags` - The flags of the received message.

### rhs.on('error', function(err){ })
```ts
rhs.on('error', (err:Error) => { /* do something */ });
```
The `error` event is emitted if there is an error with the driver.

* `err` - The error.

### Exported Constants

#### version
The actual version of the module.

#### RH_SERIAL_MAX_MESSAGE_LEN = 60
The maximum supported message length.
This is the maximum size for a Buffer used for sending or receiving messages.

## Advanced usage
`radiohead-serial` also exports classes `RH_Serial`, `RHDatagram` and `RHReliableDatagram` and some additional constants.
They represent the same classes from the native RadioHead library.
You can use them to create custom implementations.

For more information see [ADVANCED_USAGE.md](https://git.cryhost.de/crycode/node-radiohead-serial/tree/master/ADVANCED_USAGE.md)


## License

Licensed under GPL Version 2

Copyright (c) 2017 Peter Müller <peter@crycode.de> (https://crycode.de/)

The RadioHead library is Copyright (C) 2008 Mike McCauley.
