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

#endif 