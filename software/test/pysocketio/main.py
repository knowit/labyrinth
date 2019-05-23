import socketio

# standard Python
sio = socketio.Client()

sio.connect('http://localhost:9090')

@sio.on('connect')
def on_connect():
    print('I\'m connected!')
    sio.emit('gamestate', {'name': 'gamestarted'})
    sio.wait();
    sio.disconnect();
    exit();
