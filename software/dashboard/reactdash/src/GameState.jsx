import React, {Component} from "react";
import 'typeface-roboto';
import HighScoreNew from './components/HighScoreNew';
import HighScoreRow from './components/HighScoreRow';
import styles from './GameState.module.css';

class App extends Component {

  constructor() {
    super();
    this.state = {
      gamestate: 'newhighscore', // 'unknown',
      score: -99,
      highscore: [
        {
          name: 'PEE',
          score: 23123,
        },
        {
          name: 'ASS',
          score: 69,
        },{
          name: 'PEE',
          score: 23123,
        },
        {
          name: 'ASS',
          score: 69,
        },{
          name: 'PEE',
          score: 23123,
        },
        {
          name: 'ASS',
          score: 69,
        },{
          name: 'PEE',
          score: 23123,
        },
        {
          name: 'ASS',
          score: 69,
        },{
          name: 'PEE',
          score: 23123,
        },
        {
          name: 'ASS',
          score: 69,
        },
      ],
    };
  }

  componentDidMount() {
    this.props.socket.on('gamestate', data => {
      console.log(`gamestate: ${JSON.stringify(data)} `);

      var t = this;
      fetch('http://localhost:8080/highscore')
        .then(function (response) {
          return response.json();
        })
        .then(function (myJson) {
          console.log(JSON.stringify(myJson));
          t.setState({ highscore: myJson })
        });

      this.setState({ gamestate: data.name })
    });
    this.props.socket.on('score', data => {
      console.log(`score: ${JSON.stringify(data)} `);
      this.setState({ score: data.value })
    });
  }

  render() {
    const { gamestate, score, highscore } = this.state;

    const getGameState = (state) => {
      switch(state) {
        case 'newhighscore':
          return 'WON';
        case 'unknown':
          return 'IDLE';
        default:
          return 'IDLE';
      }
    }

    return (
      <div className={styles.root}>
        {gamestate === 'newhighscore' ?
          <HighScoreNew score={this.state.score} /> : ''}

        <div className={styles.gameStatus}>
          <div className={styles.scoreTitle}>
            Score
          </div>
          <div className={styles.score}>
            {score}
          </div>
          <div className={styles.gameState}>
            {getGameState(gamestate)}
          </div>
        </div>
        
        <div className={styles.highScoreTable}>
          {highscore.sort((a, b) => b.score - a.score).map(row => (
            <HighScoreRow
              name={row.name}
              score={row.score}
            />
          ))}
        </div>
      </div>
    );
  }
}

export default App;
