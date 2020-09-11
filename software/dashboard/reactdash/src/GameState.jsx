import React, {Component} from "react";
import 'typeface-roboto';
import HighScoreNew from './components/HighScoreNew';
import HighScoreRow from './components/HighScoreRow';
import styles from './GameState.module.css';

class GameState extends Component {

  constructor() {
    super();
    this.state = {
      gamestate: 'unknown',
      score: -99,
      highscore: [],
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
          t.setState({ highscore: myJson.map((item, index) => ({
            ...item,
            id: index,
          }))
        })
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
          return 'DU VENT!';
        case 'unknown':
          return 'UKJENT';
        case 'gamepending':
          return 'VENTER';
        case 'gamestarted':
          return 'SPILLER';
        case 'gamegoal':
          return 'DU VANT!';
        case 'gamelost':
          return 'UFFDA';
        default:
          return 'VENTER';
      }
    }

    return (
      <div className={`${styles.root} ${this.props.visible ? '' : styles.invisible}`}>
        {gamestate === 'newhighscore' ?
          <HighScoreNew score={this.state.score} /> : ''}

        <div className={styles.gameStatus}>
          <div className={styles.scoreTitle}>
            Poeng
          </div>
          <div className={styles.score}>
            {score}
          </div>
          <div className={styles.gameState}>
            {getGameState(gamestate)}
          </div>
        </div>
        
        <div className={styles.highScoreTable}>
          {highscore.sort((a, b) => b.score - a.score).slice(0, 10).map(row => (
            <HighScoreRow
              name={row.name}
              score={row.score}
              key={row.id}
            />
          ))}
        </div>
      </div>
    );
  }
}

export default GameState;
