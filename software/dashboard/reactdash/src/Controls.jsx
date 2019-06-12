import React, { Component } from 'react';
import Slider from '@material-ui/lab/Slider';
import Paper from '@material-ui/core/Paper';
import Typography from '@material-ui/core/Typography';
import styles from './Controls.module.css';

class Controls extends Component {

  constructor(props) {
    super(props);
    this.state = {
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
    }
  }

  componentDidMount() {
    this.props.socket.on("xbno", data => this.setState({xbno: data.value}));
    this.props.socket.on("ybno", data => this.setState({ybno: data.value}));
    this.props.socket.on("xspeed", data => this.setState({xspeed: data.value}));
    this.props.socket.on("yspeed", data => this.setState({yspeed: data.value}));
  }

  render() {
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
    return (
      <div className={styles.root}>
        <Paper>
          <Typography variant="h5" component="h3">
            X Setpoint
          </Typography>
          <Slider
            onChange={(event, value) => {
              this.setState({xsetpoint: value});
              this.props.socket.emit("xsetpoint", {value: xsetpoint});
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
              this.props.socket.emit("xCalibration", {value: xCalibration});
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
              this.props.socket.emit("xKp", {value: xKp});
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
              this.props.socket.emit("xKi", {value: xKi});
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
              this.props.socket.emit("ysetpoint", {value: ysetpoint});
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
              this.props.socket.emit("yCalibration", {value: yCalibration});
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
              this.props.socket.emit("yKp", {value: yKp});
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
              this.props.socket.emit("yKi", {value: yKi});
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

export default Controls;
