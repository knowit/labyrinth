var fs = require('fs');

class Highscore {

  init() {
    this.entries = [
      {name: 'AAA', score: 10},
      {name: 'BBB', score: 20},
      {name: 'CCC', score: 30},
      {name: 'DDD', score: 40},
      {name: 'EEE', score: 50},
      {name: 'FFF', score: 60},
      {name: 'GGG', score: 70},
      {name: 'HHH', score: 80},
      {name: 'III', score: 90},
      {name: 'JJJ', score: 100},
    ];
  }

  save() {
    fs.writeFileSync('./highscores.json', JSON.stringify(this.entries, undefined, 2), function (err) {
      if (err) throw err;
    });
  }

  load() {
    this.entries = JSON.parse(fs.readFileSync('highscores.json', 'utf8'));
  }

  isNewHighScore(score) {
    // number of scores above given score is under 10
    return this.entries.filter(entry => entry.score >= score).length < 10;
  }

  addNewHighscore( entry) {
    this.entries.push(entry);
    this.entries.sort((entry1, entry2) => entry1.score - entry2.score )
    this.entries = this.entries.slice(-10);
  }

}

module.exports = Highscore;
