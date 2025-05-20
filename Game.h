#ifndef GAME_H
#define GAME_H

#include "State.h"
#include <Arduino.h>
#include <string.h>

class NumberGame {
public:
  static const unsigned MAX_NUMBERS = 16;
private:
  unsigned userNumber;
  unsigned targetNumber;
  unsigned userGuesses;
  unsigned correctUserGuesses;
  unsigned long startTime;
  bool numbers[MAX_NUMBERS];

public:
  NumberGame();
  void reset();
  bool checkGuess();
  unsigned getUserGuesses() const;
  unsigned getCorrectUserGuesses() const;
  bool generateNewNumber();
  unsigned getTargetNumber() const;
  unsigned getUserNumber() const;
  unsigned numberUp();
};

// IMPLEMENTAÇÕES

NumberGame::NumberGame() {
  this->reset();
}

void NumberGame::reset() {
  memset(this, 0, sizeof(NumberGame));  // cuidado: isso é perigoso se tiver ponteiros ou objetos
  randomSeed(analogRead(5));
}

bool NumberGame::checkGuess() {
  ++this->userGuesses;
  if (this->userNumber == this->targetNumber) {
    ++this->correctUserGuesses;
    return true;
  }

  return false;
}

unsigned NumberGame::getUserGuesses() cons {
  return this->userGuesses;
}

unsigned NumberGame::getCorrectUserGuesses() const {
  return this->correctUserGuesses;
}

bool NumberGame::generateNewNumber() {
  if (this->userGuesses == MAX_NUMBERS) {
    return false;
  }

  do {
    this->targetNumber = random(MAX_NUMBERS);
  } while (this->numbers[this->targetNumber]);

  this->numbers[this->targetNumber] = true;

  return true;
}

unsigned NumberGame::getTargetNumber() const {
  return this->targetNumber;
}

unsigned NumberGame::getUserNumber() const {
  return this->userNumber;
}

unsigned NumberGame::numberUp() {
  ++this->userNumber;
  this->userNumber %= MAX_NUMBERS;
  return this->userNumber;
}

#endif
