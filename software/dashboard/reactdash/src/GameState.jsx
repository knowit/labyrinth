import React, {Component} from "react";
import socketIOClient from "socket.io-client";
import 'typeface-roboto';
import Slider from '@material-ui/lab/Slider';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import Button from '@material-ui/core/Button';
import Table from '@material-ui/core/Table';
import TableBody from '@material-ui/core/TableBody';
import TableCell from '@material-ui/core/TableCell';
import TableHead from '@material-ui/core/TableHead';
import TableRow from '@material-ui/core/TableRow';
import TextField from '@material-ui/core/TextField';

class App extends Component {

  constructor() {
    super();
    this.socket = socketIOClient("http://localhost:8080");
    this.state = {
      gamestate: 'unknown',
      score: -99,
      xsetpoint: 0,
      xCalibration: 0,
      yCalibration: 0,
      xKp: 0,
      xKi: 0,
      xKd: 0,
      yKp: 0,
      yKi: 0,
      yKd: 0,
      ysetpoint: 0,
      xbno: 0,
      ybno: 0,
      xspeed: 0,
      yspeed: 0,
      highscore: [],
      name: ''
    };
  }

  componentDidMount() {
    this.socket.on('gamestate', data => {
      console.log(`gamestate: ${JSON.stringify(data)} `);

      var t = this;
      fetch('http://localhost:8080/highscore')
        .then(function (response) {
          return response.json();
        })
        .then(function (myJson) {
          console.log(JSON.stringify(myJson));
          t.setState({highscore: myJson})
        });

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
    const {xCalibration} = this.state;
    const {yCalibration} = this.state;
    const {xKp} = this.state;
    const {xKi} = this.state;
    const {xKd} = this.state;
    const {yKp} = this.state;
    const {yKi} = this.state;
    const {yKd} = this.state;
    const {ysetpoint} = this.state;
    const {xbno} = this.state;
    const {ybno} = this.state;
    const {xspeed} = this.state;
    const {yspeed} = this.state;
    const {highscore} = this.state;

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
        <Button
          variant="contained"
          onClick={() => {
            fetch('http://localhost:8080/gamepending');
          }}
        >
          Pending
        </Button>
        <Button
          variant="contained"
          onClick={() => {
            fetch('http://localhost:8080/gamegoal');
          }}
        >
          Goal
        </Button>
        <Button
          variant="contained"
          onClick={() => {
            fetch('http://localhost:8080/gamelost');
          }}
        >
          Lost
        </Button>
        <TextField
          id="standard-number"
          label="Name"
          value={this.state.name}
          onChange={(event, value) => {
            this.setState({name: event.target.value});
          }}
          InputLabelProps={{
            shrink: true,
          }}
          margin="normal"
        />
        <Button
          variant="contained"
          onClick={() => {
            console.log(this.state.name);
            fetch('http://localhost:8080/addnewhighscore', {
              method: 'post',
              headers: {
                'Content-Type': 'application/json',
              },
              body: JSON.stringify({name: this.state.name, score: score})
            }).then(function(response) {
            }).then(function(data) {
            });
          }}
        >
          Add to highscore
        </Button>
        <Paper>
          <Table>
            <TableHead>
              <TableRow>
                <TableCell>name</TableCell>
                <TableCell align="right">Score</TableCell>
              </TableRow>
            </TableHead>
            <TableBody>
              {highscore.map(row => (
                <TableRow key={row.name + row.score}>
                  <TableCell component="th" scope="row">
                    {row.name}
                  </TableCell>
                  <TableCell align="right">{row.score}</TableCell>
                </TableRow>
              ))}
            </TableBody>
          </Table>
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
            X Calibration

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({xCalibration: value});
              this.socket.emit("xCalibration", {value: xCalibration});
            }}
            value={xCalibration}
            min={-3.}
            max={+3.}
          />
          <Typography>
            {xCalibration}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            X Kp

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({xKp: value});
              this.socket.emit("xKp", {value: xKp});
            }}
            value={xKp}
            min={0.}
            max={+30.}
          />
          <Typography>
            {xKp}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            X Ki

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({xKi: value});
              this.socket.emit("xKi", {value: xKi});
            }}
            value={xKi}
            min={0.}
            max={+30.}
          />
          <Typography>
            {xKi}
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
            Y Calibration

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({yCalibration: value});
              this.socket.emit("yCalibration", {value: yCalibration});
            }}
            value={yCalibration}
            min={-3.}
            max={+3.}
          />
          <Typography>
            {yCalibration}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            Y Kp

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({yKp: value});
              this.socket.emit("yKp", {value: yKp});
            }}
            value={yKp}
            min={0.}
            max={+30.}
          />
          <Typography>
            {yKp}
          </Typography>
        </Paper>
        <Paper>
          <Typography variant="h5" component="h3">
            Y Ki

          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({yKi: value});
              this.socket.emit("yKi", {value: yKi});
            }}
            value={yKi}
            min={0.}
            max={+30.}
          />
          <Typography>
            {yKi}
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
      </div>
    );
  }
}

export default App;
