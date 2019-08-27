import React, {Component} from 'react';
import Slider from './components/Slider';
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

  handleChange(event) {
    console.log(event);
  }

  render() {
    const {xsetpoint} = this.state;
    const {xCalibration} = this.state;
    const {yCalibration} = this.state;
    const {xKp} = this.state;
    const {xKi} = this.state;
    const {yKp} = this.state;
    const {yKi} = this.state;
    const {ysetpoint} = this.state;
    const {xbno} = this.state;
    const {ybno} = this.state;
    const {xspeed} = this.state;
    const {yspeed} = this.state;
    return (
      <div className={styles.root}>
        <Slider
          onChange={(event) => {
            this.setState({xsetpoint: event.target.value});
            this.props.socket.emit("xsetpoint", {value: parseFloat(xsetpoint)});
          }}
          value={xsetpoint}
          min={-5.}
          max={+5.}
          title="X Setpoint"
        />
        <Slider
          value={xbno}
          min={-5.}
          max={+5.}
          title="X Angle"
          readOnly
        />
        <Slider
          onChange={(event) => {
            this.setState({xCalibration: event.target.value});
            this.props.socket.emit("xCalibration", {value: parseFloat(xCalibration)});
          }}
          value={xCalibration}
          min={-8.}
          max={+8.}
          title="X Calibration"
        />
        <Slider
          onChange={(event) => {
            this.setState({xKp: event.target.value});
            this.props.socket.emit("xKp", {value: parseFloat(xKp)});
          }}
          value={xKp}
          min={0.}
          max={+30.}
          title="X Kp"
        />
        <Slider
          onChange={(event) => {
            this.setState({xKi: event.target.value});
            this.props.socket.emit("xKi", {value: parseFloat(xKi)});
          }}
          value={xKi}
          min={0.}
          max={+30.}
          title="X Ki"
        />
        <Slider
          value={xspeed}
          readOnly
          min={-5.}
          max={+5.}
          title="X Speed"
        />
        <Slider
          onChange={(event) => {
            this.setState({ysetpoint: event.target.value});
            this.props.socket.emit("ysetpoint", {value: parseFloat(ysetpoint)});
          }}
          value={this.state.ysetpoint}
          min={-5.}
          max={+5.}
          title="Y Setpoint"
        />
        <Slider
          value={ybno}
          min={-5.}
          max={+5.}
          title="Y Angle"
          readOnly
        />
        <Slider
          onChange={(event) => {
            this.setState({yCalibration: event.target.value});
            this.props.socket.emit("yCalibration", {value: parseFloat(yCalibration)});
          }}
          value={yCalibration}
          min={-8.}
          max={+8.}
          title="Y Calibration"
        />
        <Slider
          onChange={(event) => {
            this.setState({yKp: event.target.value});
            this.props.socket.emit("yKp", {value: parseFloat(yKp)});
          }}
          value={yKp}
          min={0.}
          max={+30.}
          title="Y Kp"
        />
        <Slider
          onChange={(event) => {
            this.setState({yKi: event.target.value});
            this.props.socket.emit("yKi", {value: parseFloat(yKi)});
          }}
          value={yKi}
          min={0.}
          max={+30.}
          title="Y Ki"
        />
        <Slider
          value={yspeed}
          min={-5.}
          max={+5.}
          title="Y Speed"
          readOnly
        />
      </div>
    );
  }

}

export default Controls;
