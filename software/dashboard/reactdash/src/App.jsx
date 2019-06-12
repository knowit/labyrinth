import React, { Component } from 'react';
import GameStatus from './GameState';
import Controls from './Controls';
import socketIOClient from "socket.io-client";

class App extends Component {
  
  constructor() {
    super();
    this.socket = socketIOClient("http://localhost:8080");
  }

  render() {
    return (
      <div className="App">
        <Controls socket={this.socket} />
        <GameStatus socket={this.socket} />
      </div>
    );
  }
  
}

export default App;
