#ifndef STATE_H
#define STATE_H

typedef enum State_ {
  STATE_INIT,
  STATE_START,
  STATE_GENERATE_NUMBER,
  STATE_WAITING_USER,
  STATE_NUM_UP,
  STATE_CHECK_GUESS,
  STATE_CORRECT_GUESS,
  STATE_WRONG_GUESS,
  STATE_TIME_UP,
  STATE_FINISH
} State;

void printStateSerial(State currentState)
{
  static State lastSerialState = STATE_FINISH;
  if (lastSerialState != currentState)
  {
    switch (currentState) 
    {
      case STATE_INIT:
        Serial.println("STATE_INIT");
        break;
      case STATE_START:
      	Serial.println("STATE_START");
        break;
      case STATE_GENERATE_NUMBER:
      	Serial.println("STATE_GENERATE_NUMBER");
        break;
      case STATE_WAITING_USER:
      	Serial.println("STATE_WAITING_USER");
        break;
      case STATE_NUM_UP:
      	Serial.println("STATE_NUM_UP");
        break;
      case STATE_CHECK_GUESS:
      	Serial.println("STATE_CHECK_GUESS");
        break;
      case STATE_CORRECT_GUESS:
        Serial.println("STATE_CORRECT_GUESS");
        break;
      case STATE_WRONG_GUESS:
        Serial.println("STATE_WRONG_GUESS");
        break;
      case STATE_TIME_UP:
      	Serial.println("STATE_TIME_UP");
        break;
      case STATE_FINISH:
      	Serial.println("STATE_FINISH");
        break;
      default:
      	Serial.println("STATE NOT MAPPED!!!!");
        Serial.println(currentState);
        break;
    }
    lastSerialState = currentState;
  }   
}


#endif 