#include "Arduino.h"
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "Game.h"

const int MAX_GAME_TIME = 99; 

class StateMachine {
private:
  State currentState;
  State nextState;
  bool upState = false;
  bool okState = false;
  unsigned long timer = 0;
  unsigned count;
  unsigned long remainingTime;
  unsigned long elapsedTime;
  NumberGame &game;
  const unsigned buttonUpPin;
  const unsigned buttonOkPin;
  bool checkButtonWasPressed();
public:
  StateMachine(NumberGame &game, unsigned buttonUpPin, unsigned buttonOkPin);
  State getCurrentState() const;
  void checkNextState();
  unsigned getCount() const;
  unsigned long getRemainingTime() const;
  unsigned long getElapsedTime() const;
};

StateMachine::StateMachine(NumberGame &game, unsigned buttonUpPin, unsigned buttonOkPin) : game(game), buttonUpPin(buttonUpPin), buttonOkPin(buttonOkPin) {
  pinMode(buttonUpPin, INPUT);
  pinMode(buttonOkPin, INPUT);
}

bool StateMachine::checkButtonWasPressed() {
  bool isOkButtonPressed = !digitalRead(buttonOkPin); 
  if (isOkButtonPressed && !okState) {
#ifdef LOG_SERIAL
    Serial.println("OK");
#endif /* LOG_SERIAL */
    okState = true;
    return true;
  }

  if (!isOkButtonPressed && okState) {
    okState = false;
  }
  
  bool isUpButtonPressed = !digitalRead(buttonUpPin);
  if (isUpButtonPressed && !upState) {
#ifdef LOG_SERIAL
    Serial.println("UP");
#endif /* LOG_SERIAL */
    upState = true;
    return true;
  }
  
  if (!isUpButtonPressed && upState) {
    upState = false;
  }
  
  return false;
}

State StateMachine::getCurrentState() const {
  return this->currentState;
}
  
void StateMachine::checkNextState() {
  currentState = nextState;
  switch (this->currentState) {
    case STATE_INIT:
      if (checkButtonWasPressed()) {
    	  game.reset();
        count = 4;
        nextState = STATE_START;
      }
      if (count > 300) {
        count = 0;
      }
      ++count;
      break;
    case STATE_START:
      if (count > 0 && !checkButtonWasPressed()) {
        --count;
        break;
      }
      timer = millis();
      nextState = STATE_GENERATE_NUMBER;
      break;
    case STATE_GENERATE_NUMBER:
      if (game.generateNewNumber()) {
  	    nextState = STATE_WAITING_USER;
      } else {
        nextState = STATE_FINISH;
        count = 0;
      }
      break;
    case STATE_WAITING_USER:
      elapsedTime = (millis() - timer) / 1000;
      if (elapsedTime >= MAX_GAME_TIME) {
        nextState = STATE_TIME_UP;
        count = 2;
      } else if(checkButtonWasPressed()) {
        if (okState) {
		      nextState = STATE_CHECK_GUESS;          
        } else if (upState) {
          nextState = STATE_NUM_UP;
        }
      }
      break;
    case STATE_NUM_UP:
      game.numberUp();
      nextState = STATE_WAITING_USER;
      break;
    case STATE_CHECK_GUESS:
      if (game.checkGuess()) {
        nextState = STATE_CORRECT_GUESS;
        count = 0;
      } else {
        nextState = STATE_WRONG_GUESS;
        count = 0;
      }
      break;
    case STATE_CORRECT_GUESS:
    case STATE_WRONG_GUESS:
      if (count == 10 || checkButtonWasPressed()) {
        nextState = STATE_GENERATE_NUMBER;
      }
      ++count;
      break;
    case STATE_FINISH:
      if (checkButtonWasPressed()) {
        nextState = STATE_INIT;
        count = 0;
      } else if (count > 400) {
        count = 0;
      } else {
        ++count;
      }

      break;
    case STATE_TIME_UP:
      if (count-- == 0) {
        nextState = STATE_FINISH;
        count = 0;
      }

      break;
  }
}

unsigned StateMachine::getCount() const {
  return this->count;
}

unsigned long StateMachine::getRemainingTime() const {
  return MAX_GAME_TIME - this->getElapsedTime();
}

unsigned long StateMachine::getElapsedTime() const {
  return elapsedTime;
}

#endif
