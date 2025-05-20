#ifndef SHOW_STATE_H
#define SHOW_STATE_H

#include <LiquidCrystal.h>

#include "Game.h"
#include "StateMachine.h"

static void printLevel(LiquidCrystal &lcd, unsigned points) {
  points = 16 * points / NumberGame::MAX_NUMBERS;
  if (points == 0) {
  	lcd.print("    UM JEGUE");
  } else if (points <= 4) {
  	lcd.print("    BURRINHO");
  } else if (points <= 8) {
  	lcd.print("      BOM");    
  } else if (points <= 12) {
  	lcd.print("     GENIO");
  } else {
  	lcd.print("     FODAO");
  }
}

void showCurrentState(LiquidCrystal &lcd, NumberGame &game, StateMachine &sm) 
{
  switch(sm.getCurrentState()) {
    case STATE_INIT:
      if (sm.getCount() <= 1) {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("JOGO B1TS->HEX");
      } else if (sm.getCount() <= 100) {
        if (sm.getCount() % 20 == 0) {
          lcd.setCursor(0, 1);
          lcd.print("                ");
        } else if (sm.getCount() % 10 == 0) {
          lcd.setCursor(0, 1);
          lcd.print("PRESSIONE TECLA");
        }
      } else if (sm.getCount() == 101) {
        lcd.setCursor(0, 0);
        for (int i = 0; i < 40; ++i) {
          lcd.print(i % 2);
        }
        for (int i = 0; i < 40; ++i) {
          lcd.print(i % 0x10, HEX);
        }
      } else if (sm.getCount() <= 300) {
        if (sm.getCount() % 8 == 0) {
          lcd.scrollDisplayLeft();
        }
      }

      break;
    case STATE_START:
      lcd.clear();
      if (sm.getCount() > 0) {
        lcd.setCursor(7, 0);
        lcd.print(sm.getCount());
        delay(1000);
      }
      break;
    case STATE_GENERATE_NUMBER:
      lcd.setCursor(0, 1);
      lcd.print("Valor de ");
      lcd.print(game.getTargetNumber() >> 3 & 0x1);
      lcd.print(game.getTargetNumber() >> 2 & 0x1);
      lcd.print(game.getTargetNumber() >> 1 & 0x1);
      lcd.print(game.getTargetNumber() >> 0 & 0x1);
      lcd.print(": ");
      // fall through
    case STATE_NUM_UP:
      lcd.setCursor(15, 1);      
      lcd.print(game.getUserNumber(), HEX);
      break;
    case STATE_CHECK_GUESS:
      break;
    case STATE_CORRECT_GUESS:
      if (sm.getCount() <= 1) {
        lcd.setCursor(0, 1);      
        lcd.print("    CORRETO!    ");
      } else if (sm.getCount() == 10) {
        lcd.setCursor(0, 1);      
        lcd.print("                ");
      }
      break;
    case STATE_WRONG_GUESS:
      if (sm.getCount() <= 1) {
        lcd.setCursor(0, 1);      
        lcd.print("    ERRADO!     ");
      } else if (sm.getCount() == 10) {
        lcd.setCursor(0, 1);      
        lcd.print("                ");
      }
      break;
    case STATE_WAITING_USER:
      lcd.setCursor(0, 0);
      lcd.print(sm.getRemainingTime());
      lcd.print("s");
      if (sm.getRemainingTime() < 10) {
        lcd.print(" ");
      }
      lcd.setCursor(11, 0);
      if (game.getUserGuesses() + 1 < 10) {
        lcd.print("0");  
      }
      lcd.print(game.getUserGuesses() + 1);
      lcd.print("/");
      lcd.print(NumberGame::MAX_NUMBERS);
      lcd.setCursor(0, 1);
      break;
    case STATE_TIME_UP:
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("TEMPO ESGOTADO");
      delay(1000);
      lcd.clear();
      delay(1000);
      break;
    case STATE_FINISH:
      if (sm.getCount() < 100) {
        if (sm.getCount() % 20 == 0) {
          lcd.clear();
        } else if (sm.getCount() % 10 == 0) {
          lcd.setCursor(0, 0);
          lcd.print(game.getCorrectUserGuesses());
          lcd.print(" ponto");
          if (game.getCorrectUserGuesses() != 1) {
            lcd.print("s"); // plural
          }
          lcd.print(" em ");
          lcd.print(sm.getElapsedTime());
          lcd.print("s");
        }
      } else if (sm.getCount() > 100 && sm.getCount() < 120) {
        lcd.scrollDisplayLeft();
      } else if (sm.getCount() < 130) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Parabens, voce e");
      } else if (sm.getCount() > 180) {
        lcd.setCursor(0, 1);
        if ((sm.getCount() / 10) % 2 == 0) {
          lcd.print("                ");
        } else {
          printLevel(lcd, game.getCorrectUserGuesses());
        }
      }

      break;
    default:
      break;
  }

}
#endif