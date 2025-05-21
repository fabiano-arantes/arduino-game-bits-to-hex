// C++ code
//

#include <LiquidCrystal.h>

#include "Game.h"
#include "StateMachine.h"
#include "ShowState.h"

// #define LOG_SERIAL // uncomment this line to generate logs to serial port

const byte LCD_RS_PIN = 2;
const byte LCD_E_PIN = 3;
const byte LCD_D4_PIN = 4;
const byte LCD_D5_PIN = 5;
const byte LCD_D6_PIN = 6;
const byte LCD_D7_PIN = 7;

const byte BUTTON_UP_PIN = A0;
const byte BUTTON_OK_PIN = A1;

const byte LED_0_PIN = 8;
const byte LED_1_PIN = 9;
const byte LED_2_PIN = 10;
const byte LED_3_PIN = 11;

const int DEFAULT_DELAY = 50;

LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN); // create lcd object and assign pins
NumberGame game; // create game core object
StateMachine sm(game, BUTTON_UP_PIN, BUTTON_OK_PIN); // create state machine object and assign pins
ShowState ss(lcd, game, sm, LED_0_PIN, LED_1_PIN, LED_2_PIN, LED_3_PIN); // create show state object and assign pins

#ifdef LOG_SERIAL
void printStateSerial(State currentState)
{
  static State lastSerialState = STATE_FINISH;
  if (lastSerialState != currentState) {
    switch (currentState) {
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
#endif // LOG_SERIAL

void setup()
{
  lcd.begin(16, 2);
  game.reset();  
#ifdef LOG_SERIAL
  Serial.begin(9600);
#endif /* LOG_SERIAL */
}
  
void loop()
{
#ifdef LOG_SERIAL
  printStateSerial(sm.getCurrentState());
#endif /* LOG_SERIAL */

  sm.checkNextState();
  ss.showCurrentState();

  delay(DEFAULT_DELAY);
}
