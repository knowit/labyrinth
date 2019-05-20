import React, { Component } from "react";
import socketIOClient from "socket.io-client";
class App extends Component {
  constructor() {
    super();
    this.state = {
      gameState: false,
    };
  }
  componentDidMount() {
    const { endpoint } = this.state;
    const socket = socketIOClient("http://127.0.0.1:9090");
    socket.on("event", data => this.setState({ gameState: data.name }));
    socket.on("score", data => this.setState({ score: data.value }));
  }
  render() {
    const { gameState } = this.state;
    const { score } = this.state;
    return (
      <div style={{ textAlign: "center" }}>
        {gameState
          ? <p>
            Game state: {gameState}
          </p>
          : <p>Waiting for game state event...</p>}
        {score
          ? <p>
            score: {score}
          </p>
          : <p>score: 0
          </p>}
      </div>
    );
  }
}
export default App;
