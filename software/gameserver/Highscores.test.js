
test('basic functionality', () => {
  const Highscore = require('./Highscore');

  let highscore = new Highscore();
  highscore.init();

  expect(highscore.isNewHighScore(2)).toBe(false);
  expect(highscore.isNewHighScore(10)).toBe(false);
  expect(highscore.isNewHighScore(15)).toBe(true);
  expect(highscore.isNewHighScore(50)).toBe(true);
  expect(highscore.isNewHighScore(200)).toBe(true);

  highscore.addNewHighscore({name:'NW1', score: 300})
  highscore.addNewHighscore({name:'NW2', score: 66})

  expect(highscore.entries.length).toBe(10);

  highscore.save();

  highscore.init();
  console.log(highscore.entries);
  highscore.load();

  console.log(highscore.entries);
});


