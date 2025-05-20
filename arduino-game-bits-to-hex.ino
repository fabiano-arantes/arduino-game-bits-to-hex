// C++ code
//

#include <LiquidCrystal.h>
#include "Game.h"
#include "StateMachine.h"
#include "ShowState.h"

// #define LOG_SERIAL

const int BUTTON_UP_PIN = A0;
const int BUTTON_OK_PIN = A1;
const int DEFAULT_DELAY = 50;

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // cria o objeto lcd e atribui os pinos
NumberGame game;
StateMachine sm(game, BUTTON_UP_PIN, BUTTON_OK_PIN);

int count = 0;

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
  printStateSerial();
#endif /* LOG_SERIAL */

  sm.checkNextState();
  showCurrentState(lcd, game, sm);

  delay(DEFAULT_DELAY);
}