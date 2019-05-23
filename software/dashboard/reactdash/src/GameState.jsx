import React, {Component} from "react";
import socketIOClient from "socket.io-client";
import 'typeface-roboto';
import Slider from '@material-ui/lab/Slider';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';

class App extends Component {

  constructor() {
    super();
    this.socket = socketIOClient("http://127.0.0.1:9090");
    this.state = {
      gamestate: 'unknown',
      score: -99,
      xsetpoint: 0,
      ysetpoint: 0,
      xbno: 0,
      ybno: 0,
      xspeed: 0,
      yspeed: 0,
    };
  }

  componentDidMount() {
    this.socket.on('gamestate', data => {
      console.log(`gamestate: ${JSON.stringify(data)} `);
      this.setState({gamestate: data.name})
    });
    this.socket.on('score', data => {
      console.log(`score: ${JSON.stringify(data)} `);
      this.setState({score: data.value})
    });
    this.socket.on("xbno", data => this.setState({xbno: data.value}));
    this.socket.on("ybno", data => this.setState({ybno: data.value}));
    this.socket.on("xspeed", data => this.setState({xspeed: data.value}));
    this.socket.on("yspeed", data => this.setState({yspeed: data.value}));
  }

  render() {
    const {gamestate} = this.state;
    const {score} = this.state;
    const {xsetpoint} = this.state;
    const {ysetpoint} = this.state;
    const {xbno} = this.state;
    const {ybno} = this.state;
    const {xspeed} = this.state;
    const {yspeed} = this.state;
    return (
      <div>
        {gamestate
          ? <p>
            Game state: {gamestate}
          </p>
          : <p>Waiting for game state event...</p>}
        {score
          ? <p>
            score: {score}
          </p>
          : <p>score: 0
          </p>}
        <Paper>
          <Typography variant="h5" component="h3">
            X Angle
          </Typography>
          <Slider
            value={xbno}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {xbno}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            Y Angle
          </Typography>
          <Slider
            value={ybno}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {ybno}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            X Speed
          </Typography>
          <Slider
            value={xspeed}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {xspeed}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            Y Speed
          </Typography>
          <Slider
            value={yspeed}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {yspeed}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            X Setpoint

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({xsetpoint: value});
              this.socket.emit("xsetpoint", {value: xsetpoint});
            }}
            value={xsetpoint}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {xsetpoint}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            Y Setpoint
          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({ysetpoint: value});
              this.socket.emit("ysetpoint", {value: ysetpoint});
            }}
            value={ysetpoint}
            min={-5.}
            max={+5.}
          />
          <Typography>
            {ysetpoint}
          </Typography>
        </Paper>
      </div>
    );
  }
}

export default App;
