import socketio
import time
import json

sio = socketio.Client()

@sio.event
def connect():
    print('Socket.io connection established')

@sio.event
def my_message(data):
    print('Message received with ', data)

@sio.event
def disconnect():
    print('disconnected from server')

sio.connect('http://10.170.18.126:8080')
#sio.wait()

x_direction = 1
y_direction = 1
x_angle = -3
y_angle = 3

while True:
  x_angle = float("{0:.2f}".format(x_angle + x_direction * 0.1))
  y_angle = float("{0:.2f}".format(y_angle + y_direction * 0.1))
  if (x_angle * x_direction > 3): x_direction *= -1
  if (y_angle * y_direction > 3): y_direction *= -1
  msg = {
    "x": x_angle,
    "y": y_angle,
  }
  print(json.dumps(msg))
  try:
    sio.emit('pos', msg)
  except e:
    print(e)
  time.sleep(1)
