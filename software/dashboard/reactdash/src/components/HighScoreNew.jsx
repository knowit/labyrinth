import React, { Component } from 'react';
import styles from './HighScoreNew.module.css';

class HighScoreNew extends Component {

  constructor(props) {
    super(props);
    this.state = {
      name: '',
    }
  }
  
  render() {
    return (
      <div className={styles.root}>
        <div className={styles.title}>
          New highscore!
        </div>
        <div className={styles.userInput}>
          <input
            className={styles.input}
            label="Name"
            value={this.state.name}
            onChange={(event, value) => {
              this.setState({ name: event.target.value });
            }}
            InputLabelProps={{
              shrink: true,
            }}
            margin="normal"
          />
          <button
            className={styles.button}
            onClick={() => {
              console.log(this.state.name);
              const t = this;
              fetch('http://localhost:8080/addnewhighscore', {
                method: 'post',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify({ name: this.state.name, score: this.props.score })
              }).then(function(response) {
              }).then(function(data) {
                t.setState({ name: '' });
              });
            }}
          >
            Add to highscore
          </button>
        </div>
      </div>
    )
  }
}

export default HighScoreNew;
