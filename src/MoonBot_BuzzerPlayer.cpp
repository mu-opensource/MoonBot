/*
 * MoonBot_BuzzerPlayer.cpp
 *
 *  Created on: 2019年5月29日
 *      Author: ysq
 */

#include "MoonBot_BuzzerPlayer.h"

void buzzerPlayer(const unsigned int notation[][2],
                  const size_t length,
                  unsigned int beat_ms) {
  uint8_t pin = MOONBOT_PIN_BUZZER_SIG;
  for (size_t i = 0; i < length; ++i) {
    if (notation[i][0]) {
      tone(pin, notation[i][0]);
    }
    delay(notation[i][1]*beat_ms/BEAT_FRACTION_WHOLE);
    noTone(pin);
  }
}

void buzzerPlayerPGM(const unsigned int notation[][2],
                     const size_t length,
                     unsigned int beat_ms) {
  uint8_t pin = MOONBOT_PIN_BUZZER_SIG;
  for (size_t i = 0; i < length; ++i) {
    unsigned int frequency = pgm_read_word(&notation[i][0]);
    unsigned int duration = pgm_read_word(&notation[i][1]);
    if (frequency) {
      tone(pin, frequency);
    }
    delay(duration*beat_ms/BEAT_FRACTION_WHOLE);
    noTone(pin);
  }
}


