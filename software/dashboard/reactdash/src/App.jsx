import React, { Component } from 'react';
import GameStatus from './GameState';
import Controls from './Controls';
import socketIOClient from "socket.io-client";
import styles from './App.module.css';

class App extends Component {
  
  constructor() {
    super();
    this.socket = socketIOClient("http://localhost:8080");
    this.state = {
      gameStateOnTop: true,
    };
  }

  render() {
    return (
      <div className="App">
        <button
          className={styles.toggleButton}
          onClick={() => this.setState(prevState => ({ gameStateOnTop: !prevState.gameStateOnTop }))}
        >
          {this.state.gameStateOnTop ? 'Hide' : 'Show'}
        </button>
        <Controls socket={this.socket} />
        <GameStatus socket={this.socket} visible={this.state.gameStateOnTop} />
      </div>
    );
  }
  
}

export default App;
