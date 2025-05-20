#ifndef SHOW_STATE_H
#define SHOW_STATE_H

#include "Arduino.h"
#include <LiquidCrystal.h>

#include "Game.h"
#include "StateMachine.h"

#define LED_0_PIN 8
#define LED_1_PIN 9
#define LED_2_PIN 10
#define LED_3_PIN 11

typedef enum {
  LEDS_ONLY_LED_0,
  LEDS_ONLY_LED_1,
  LEDS_ONLY_LED_2,
  LEDS_ONLY_LED_3,
  LEDS_TURN_ALL_ON,
  LEDS_TURN_ALL_OFF
} LedsState;

void ledsValue(byte v)
{
  digitalWrite(LED_0_PIN, v >> 0 & 0x1);
  digitalWrite(LED_1_PIN, v >> 1 & 0x1);
  digitalWrite(LED_2_PIN, v >> 2 & 0x1);
  digitalWrite(LED_3_PIN, v >> 3 & 0x1);
}
void ledsAnimation(LedsState s) 
{
  switch (s)
  {
    case LEDS_ONLY_LED_0:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(LED_0_PIN, HIGH);
      break;
    case LEDS_ONLY_LED_1:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(LED_1_PIN, HIGH);
      break;
    case LEDS_ONLY_LED_2:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(LED_2_PIN, HIGH);
      break;
    case LEDS_ONLY_LED_3:
      ledsAnimation(LEDS_TURN_ALL_OFF);
      digitalWrite(LED_3_PIN, HIGH);
      break;
    case LEDS_TURN_ALL_ON:
      digitalWrite(LED_0_PIN, HIGH);
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, HIGH);
      digitalWrite(LED_3_PIN, HIGH);
      break;
    case LEDS_TURN_ALL_OFF:
      digitalWrite(LED_0_PIN, LOW);
      digitalWrite(LED_1_PIN, LOW);
      digitalWrite(LED_2_PIN, LOW);
      digitalWrite(LED_3_PIN, LOW);
      break;
  }
}

class ShowState {
private:
  LiquidCrystal &lcd;
  const NumberGame &game;
  const StateMachine &sm;
public:
  ShowState(LiquidCrystal &lcd, const NumberGame &game, const StateMachine &sm);
  void showCurrentState();
};

static void printLevel(LiquidCrystal &lcd, unsigned points) {
  points = 16 * points / NumberGame::MAX_NUMBERS;
  if (points == 0) {
  	lcd.print("    UM JEGUE");
  } else if (points <= 2) {
  	lcd.print("  BEM BURRINHO");
  } else if (points <= 5) {
  	lcd.print("     BURRO");
  } else if (points <= 8) {
  	lcd.print("   MEIO BURRO");
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

ShowState::ShowState(LiquidCrystal &lcd, const NumberGame &game, const StateMachine &sm) : lcd(lcd), game(game), sm(sm) 
{
  pinMode(LED_0_PIN, OUTPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
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
      break;
    case STATE_CHECK_GUESS:
      break;
    case STATE_CORRECT_GUESS:
      if (sm.getCount() <= 1) {
        lcd.setCursor(0, 1);      
        lcd.print("    CORRETO!    ");
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() == 2) {
        digitalWrite(LED_3_PIN, HIGH);
      } else if (sm.getCount() == 4) {
        digitalWrite(LED_2_PIN, HIGH);
      } else if (sm.getCount() == 6) {
        digitalWrite(LED_1_PIN, HIGH);
      } else if (sm.getCount() == 8) {
        digitalWrite(LED_0_PIN, HIGH);
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
      ledsAnimation(LEDS_TURN_ALL_ON);
      delay(1000);
      lcd.clear();
      ledsAnimation(LEDS_TURN_ALL_OFF);
      delay(1000);
      break;
    case STATE_FINISH:
      if (sm.getCount() <= 1) {
        lcd.clear();
        ledsAnimation(LEDS_TURN_ALL_OFF);
      } else if (sm.getCount() < 100) { // mostra pontos e tempo decorrido
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
      } else if (sm.getCount() >= 100 && sm.getCount() < 120) { // roda tela pra esquerda
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
          digitalWrite(LED_0_PIN, HIGH);
          digitalWrite(LED_3_PIN, HIGH);
          lcd.print("                ");
        } else {
          digitalWrite(LED_1_PIN, HIGH);
          digitalWrite(LED_2_PIN, HIGH);
          printLevel(lcd, game.getCorrectUserGuesses());
        }
      }

      break;
    default:
      break;
  }

}
#endif
