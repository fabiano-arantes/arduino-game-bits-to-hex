#ifndef SHOW_STATE_H
#define SHOW_STATE_H

#include "Game.h"
#include "StateMachine.h"

typedef enum {
  LEDS_ONLY_LED_0,
  LEDS_ONLY_LED_1,
  LEDS_ONLY_LED_2,
  LEDS_ONLY_LED_3,
  LEDS_TURN_ALL_ON,
  LEDS_TURN_ALL_OFF
} LedsState;

class ShowState {
private:
  LiquidCrystal &lcd;
  const NumberGame &game;
  const StateMachine &sm;
  const byte led0Pin;
  const byte led1Pin;
  const byte led2Pin;
  const byte led3Pin;
  void printLevel();
  void ledsAnimation(LedsState s);
  void ledsValue(byte v);
public:
  ShowState(LiquidCrystal &lcd, const NumberGame &game, const StateMachine &sm, byte led0Pin,
            byte led1Pin, byte led2Pin, byte led3Pin);
  void showCurrentState();
};

ShowState::ShowState(LiquidCrystal &lcd, const NumberGame &game, const StateMachine &sm, byte led0Pin,
                     byte led1Pin, byte led2Pin, byte led3Pin) : 
                     lcd(lcd), game(game), sm(sm), led0Pin(led0Pin), led1Pin(led1Pin), led2Pin(led2Pin),
                     led3Pin(led3Pin) 
{
  pinMode(led0Pin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
}

void ShowState::printLevel()
{
  unsigned points = 16 * game.getCorrectUserGuesses() / NumberGame::MAX_NUMBERS;
  if (points == 0) {
  	lcd.print("    UM JEGUE");
  } else if (points <= 2) {
  	lcd.print("  BEM BURRINHO");
  } else if (points <= 5) {
  	lcd.print("     BURRO");
  } else if (points <= 8) {
  	lcd.print("    MEDIANO");
  } else if (points <= 11) {
  	lcd.print("      BOM");    
  } else if (points <= 14) {
  	lcd.print("  INTELIGENTE");    
  } else if (points == 15) {
  	lcd.print("     GENIO");
  } else {
  	lcd.print("     FODAO");
  }
}

void ShowState::ledsValue(byte v)
{
  digitalWrite(led0Pin, v >> 0 & 0x1);
  digitalWrite(led1Pin, v >> 1 & 0x1);
  digitalWrite(led2Pin, v >> 2 & 0x1);
  digitalWrite(led3Pin, v >> 3 & 0x1);
}
void ShowState::ledsAnimation(LedsState s)
{
  switch (s) {
    case LEDS_ONLY_LED_0:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(led0Pin, HIGH);
      break;
    case LEDS_ONLY_LED_1:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(led1Pin, HIGH);
      break;
    case LEDS_ONLY_LED_2:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(led2Pin, HIGH);
      break;
    case LEDS_ONLY_LED_3:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(led3Pin, HIGH);
      break;
    case LEDS_TURN_ALL_ON:
      digitalWrite(led0Pin, HIGH);
      digitalWrite(led1Pin, HIGH);
      digitalWrite(led2Pin, HIGH);
      digitalWrite(led3Pin, HIGH);
      break;
    case LEDS_TURN_ALL_OFF:
      digitalWrite(led1Pin, LOW);
      digitalWrite(led0Pin, LOW);
      digitalWrite(led2Pin, LOW);
      digitalWrite(led3Pin, LOW);
      break;
  }
}

void ShowState::showCurrentState() 
{
  switch(sm.getCurrentState()) {
    case STATE_INIT:
      if (sm.getCount() <= 1) {
        ledsAnimation(LEDS_TURN_ALL_OFF);
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("JOGO B1TS->HEX");
      } else if (sm.getCount() <= 100) {
        if (sm.getCount() % 20 == 0) {
          lcd.setCursor(0, 1);
          lcd.print("                ");
          ledsAnimation(LEDS_TURN_ALL_OFF);
        } else if (sm.getCount() % 10 == 0) {
          lcd.setCursor(0, 1);
          lcd.print("PRESSIONE TECLA");
          ledsAnimation(LEDS_TURN_ALL_ON);
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
        ledsAnimation(static_cast<LedsState>(sm.getCount() % 4));
      }

      break;
    case STATE_START:
      lcd.clear();
      if (sm.getCount() > 0) {
        ledsAnimation(static_cast<LedsState>(sm.getCount() - 1));
        lcd.setCursor(7, 0);
        lcd.print(sm.getCount());
        delay(1000);
      }
      break;
    case STATE_GENERATE_NUMBER:
      ledsValue(game.getTargetNumber());
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
      lcd.blink();
      break;
    case STATE_CHECK_GUESS:
      lcd.noBlink();
      break;
    case STATE_CORRECT_GUESS:
      if (sm.getCount() <= 1) {
        lcd.setCursor(0, 1);      
        lcd.print("    CORRETO!    ");
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() == 2) {
        digitalWrite(led3Pin, HIGH);
      } else if (sm.getCount() == 4) {
        digitalWrite(led2Pin, HIGH);
      } else if (sm.getCount() == 6) {
        digitalWrite(led1Pin, HIGH);
      } else if (sm.getCount() == 8) {
        digitalWrite(led0Pin, HIGH);
      } else if (sm.getCount() == 10) {
        ledsAnimation(LEDS_TURN_ALL_OFF);
        lcd.setCursor(0, 1);      
        lcd.print("                ");
      }
      break;
    case STATE_WRONG_GUESS:
      if (sm.getCount() <= 1) {
        lcd.setCursor(0, 1);      
        lcd.print("    ERRADO!     ");
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() % 2 != 0) {
        ledsAnimation(LEDS_TURN_ALL_ON);
      } else {
        ledsAnimation(LEDS_TURN_ALL_OFF);
        if (sm.getCount() == 10) {
          lcd.setCursor(0, 1);      
          lcd.print("                ");
        }
      }
      break;
    case STATE_WAITING_USER:
      static unsigned long lastTime = 0;
      static bool showPoints = false;

      if (sm.getElapsedTime() - lastTime >= 3) {
        lastTime = sm.getElapsedTime();
        showPoints = !showPoints;
      }

      lcd.setCursor(0, 0);
      if (!showPoints) {
        lcd.print("Rodada ");
        lcd.print(game.getUserGuesses() + 1);
        lcd.print(" de ");
        lcd.print(NumberGame::MAX_NUMBERS);
        lcd.print("   ");
      } else {
        lcd.print(sm.getRemainingTime());
        lcd.print("s    ");

        if (game.getUserGuesses() < 10) {
          lcd.setCursor(7, 0);
        } else {
          lcd.setCursor(6, 0);  
        }
        
        lcd.print("Pontos: ");
        lcd.print(game.getCorrectUserGuesses());
      }
      lcd.setCursor(15, 1);
      break;
    case STATE_TIME_UP:
      lcd.noBlink();
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("TEMPO ESGOTADO");
      ledsAnimation(LEDS_TURN_ALL_ON);
      delay(1000);
      lcd.clear();
      ledsAnimation(LEDS_TURN_ALL_OFF);
      delay(1000);
      break;
    case STATE_FINISH:
      lcd.noBlink();
      if (sm.getCount() <= 1) {
        lcd.clear();
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() < 100) { // show points and elapsed time
        if (sm.getCount() % 20 == 0) { 
          lcd.clear();
          ledsAnimation(LEDS_TURN_ALL_OFF);
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
          ledsAnimation(LEDS_TURN_ALL_ON);
        }
      } else if (sm.getCount() >= 100 && sm.getCount() < 120) { // rotate screen to left
        lcd.scrollDisplayLeft();

        // acenderá somente 1 led de cada vez dentro do período
        ledsAnimation(static_cast<LedsState>((sm.getCount() / 5) % 4)); 
      } else if (sm.getCount() < 130) { // escreve primeira linha com parabens
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Parabens, voce e");
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() < 140) {
        ledsAnimation(LEDS_ONLY_LED_3);
      } else if (sm.getCount() < 150) {
        ledsAnimation(LEDS_ONLY_LED_2);
      } else if (sm.getCount() < 160) {
        ledsAnimation(LEDS_ONLY_LED_1);
      } else if (sm.getCount() < 170) {
        ledsAnimation(LEDS_ONLY_LED_0);
      } else if (sm.getCount() < 180) {
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() > 190) { // escreve segunda linha piscante com nivel de inteligencia
        lcd.setCursor(0, 1);
        ledsAnimation(LEDS_TURN_ALL_OFF);
        if ((sm.getCount() / 10) % 2 == 0) {
          digitalWrite(led0Pin, HIGH);
          digitalWrite(led3Pin, HIGH);
          lcd.print("                ");
        } else {
          digitalWrite(led1Pin, HIGH);
          digitalWrite(led2Pin, HIGH);
          printLevel();
        }
      }

      break;
    default:
      break;
  }

}
#endif
