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
          <form onSubmit={(event) => {
            event.preventDefault();
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
            })
          }}
          >
            <input
              autoFocus
              className={styles.input}
              label="Name"
              value={this.state.name}
              onChange={(event, value) => {
                if (event.target.value.length > 3) return;
                this.setState({ name: event.target.value.toUpperCase() });
              }}
              InputLabelProps={{
                shrink: true,
              }}
              margin="normal"
            />
            <button
              type="submit"
              className={styles.button}
            >
              Add to highscore
            </button>
          </form>
        </div>
      </div>
    )
  }
}

export default HighScoreNew;
