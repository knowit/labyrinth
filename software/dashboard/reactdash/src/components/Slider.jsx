import React from 'react';
import styles from './Slider.module.css';

const Slider = (props) => (
  <div className={styles.root}>
    <div className={styles.title}>
      {props.title}
    </div>
    <input
      className={styles.slider}
      type="range"
      min={props.min}
      max={props.max}
      value={props.value}
      onChange={props.onChange}
      readOnly={props.readOnly}
      step={0.1}
    />
    <div className={styles.value}>
      {props.value}
    </div>
  </div>
);

export default Slider;
