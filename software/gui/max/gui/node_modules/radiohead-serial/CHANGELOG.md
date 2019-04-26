## v4.0.0 2018-10-08
- Upgraded all dependencies to use prebuild binaries and fix known vulnerabilities
- BRAKING: v4 of radiohead-serial requires at least Node.js 8! For older released of Node.js use the latest v3.
- Build results are now in ES6 style

## v3.1.0 2017-06-15
- Error events from the SerialPort are now proxied to RH_Serial and RadioHeadSerial classes

## v3.0.1 2017-06-03
- Recode to use only TS/JS code and dropped the native addon part
- All needed classes from the RadioHead library are now ported to TypeScript!
- Linux, Windows and Mac are now supported
- New APIv3 mostly compatible to APIv2
- New advanced usage possibilty using the exported `RH_Serial`, `RHDatagram` and `RHReliableDatagram` classes
- Added constants RH_SERIAL_MAX_MESSAGE_LEN, RH_SERIAL_MAX_PAYLOAD_LEN, RH_SERIAL_HEADER_LEN, RH_FLAGS_NONE, RH_FLAGS_RESERVED, RH_FLAGS_APPLICATION_SPECIFIC, RH_FLAGS_ACK, RH_BROADCAST_ADDRESS, RH_DEFAULT_TIMEOUT, RH_DEFAULT_RETRIES
- Added serial-rf95 gateway arduino sketch
- Updated tests
- Updated TypeScript typings

## v2.0.0 (not released)
- New APIv2 with breaking changes to use Events and Promises
- Added support for multiple instances of the RadioHeadSerial class
- Added more information for received messages (length, from, to, id, flags)
- Added support for the promiscuous mode of the receiver, to receive messages with any to address
- Added ability to change the sleep time of the asynchronous worker
- Added constant MAX_MESSAGE_LEN
- Updated TypeScript typings
- Added TypeScript examples
- Added tests

## v1.0.4 2017-03-21
- Updated RadioHead library to v1.74

## v1.0.3 2017-02-28
- Finally fixed segmentation fault bug
- Added serial gateway description and example arduino sketch

## v1.0.2 2017-02-15
- Fixed segmentation fault bug

## v1.0.1 2016-12-19
- Added getRetries(), getRetransmissions() and resetRetransmissions()

## v1.0.0 2016-12-15
- First official release
