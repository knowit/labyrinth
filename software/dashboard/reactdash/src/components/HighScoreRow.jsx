import React from 'react';
import styles from './HighScoreRow.module.css';

const HighScoreRow = (props) => (
  <div className={styles.root}>
    <div className={styles.name}>
      {props.name}
    </div>
    <div className={styles.score}>
      {props.score}
    </div>
  </div>
)

export default HighScoreRow;
