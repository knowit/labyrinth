/*
 * Node.js module radiohead-serial
 *
 * Copyright (c) 2017 Peter Müller <peter@crycode.de> (https://crycode.de/)
 *
 * Node.js module for communication between some RadioHead nodes and Node.js using
 * the RH_Serial driver and the RHReliableDatagram manager of the RadioHead library.
 *
 *
 * RadioHead Library (http://www.airspayce.com/mikem/arduino/RadioHead/)
 * Copyright (c) 2014 Mike McCauley
 *
 * Port from native C/C++ code to TypeScript
 * Copyright (c) 2017 Peter Müller <peter@crycode.de> (https://crycode.de/)
 *
 * Example for the communiation between two nodes.
 * This client example sends ten messages to the server and prints all received messages.
 */

///<reference types="node" />

// Import the radiohead-serial module
//import {RadioHeadSerial, RH_ReceivedMessage} from 'radiohead-serial';
// @ts-ignore
import {RadioHeadSerial, RH_ReceivedMessage} from '../';

// Create an instance of the RadioHeadSerial class
let rhs:RadioHeadSerial = new RadioHeadSerial('/dev/ttyUSB1', 9600, 0x02);
//let rhs:RadioHeadSerial = new RadioHeadSerial('COM2', 9600, 0x02);

// Listen on the 'data' event for received messages
rhs.on('data', (message:RH_ReceivedMessage) => {
  // Print the received message object
  console.log('-> recv:', message);

  // Convert the decimal from address to hex
  let sender:string = ('0' + message.headerFrom.toString(16)).slice(-2).toUpperCase();

  // Print a readable form of the data
  if(message.length > 0){
    console.log('-> received ' + message.length + ' bytes from 0x' + sender + ': "' + message.data.toString() + '"');
  }
});

// Counter for the number of send messages
let sentCount:number = 0;

// Function to send a message (calls itself with a timeout until five messages are sent)
function sendData(){
  // Create the data to be send to the server
  let data:Buffer = Buffer.from('Hello server!');

  // Send the data to the server
  rhs.send(0x01, data).then(() => {
    // Message has been sent successfully
    console.log('<- sent to 0x01: "' + data.toString() + '" Raw:', data);

  }).catch((error:Error) => {
    // Error while sending the message
    console.log('<- ERROR', error);

  }).finally(() => {
    // After sending the message, even if failed
    sentCount++;

    // 5 times sent?
    if(sentCount < 5){
      // Send a new message after 2 seconds
      setTimeout(sendData, 2000);
    }else{
      // Close the SerialPort worker after 1 second and exit the client example
      // Use the timeout before close() to receive the answer from the server
      setTimeout(()=>{
        rhs.close().then(()=>{
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
