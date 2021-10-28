// 4 buttons, 6 LEDs
// - 2 buttons for lunch & dinner
// - 2 buttons to toggle days
// - 4 LEDs for 2 days worth of lunch & dinner
// - 2 LEDs to indicate which day

// include neopixel library
#include <Adafruit_NeoPixel.h>

#define LED_PIN      3
#define N_LEDS 7
#define numButtons 4
#define numOptions 2
#define numDays 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// option of lunch & dinner
const int allButtons[numButtons] = {8, 13, 12, 11};
const int optionButtons[numOptions] = {12, 11};
const uint32_t optionColors[numOptions] = {strip.Color(0, 255, 0),  strip.Color(255, 128, 0)};
// light up which day it is
const int dateLEDs[numDays] = {1, 6}; // M, Tu

// remember which LEDs are lit up
int ledStates[numDays][numOptions];
int date = 0;
// previous button states
int prevButtonState[numButtons] = {HIGH, HIGH, HIGH, HIGH};

void setup() {
  // begin Neopixel strip
  strip.begin();

  // set button pin modes
  for (int i = 0; i < numButtons; i += 1) {
    pinMode(allButtons[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
}

void loop() {
  readButtons();
  updatePixels();
}

void readButtons() {
  // see if any are pressed
  for (int button = 0; button < numButtons; button += 1) {
    int buttonReading = digitalRead(allButtons[button]);

    if (buttonReading == LOW && buttonReading != prevButtonState[button]) {
      // if button state has changed & it's been pressed
      // (LOW because of PULLUP)

      if (button == 0) {
        // if it's back button
        date = constrain(date - 1, 0, numDays - 1);
      } else if (button == 1) {
        // if forward button
        date = constrain(date + 1, 0, numDays - 1);
      } else {
        int option = button - 2;
        ledStates[date][option] = !ledStates[date][option];
      }
    }

    // remember button state
    prevButtonState[button] = buttonReading;
  }
}

void updatePixels() {

  for (int day = 0; day < numDays; day += 1) {
    // light up correct day
    if (day == date) {
      strip.setPixelColor(dateLEDs[day], strip.Color(255, 0, 0));
    } else {
      strip.setPixelColor(dateLEDs[day], 0);
    }

    // light up correct leds
    for (int option = 0; option < numOptions; option += 1) {
      int pos = option + 2;
      if (day == 1) {
        // something weird I had to do bc of how Neopixel is wired
        pos = 5 - option; 
      }
      boolean lightup = ledStates[day][option];

      if (lightup) {
        strip.setPixelColor(pos, optionColors[option]);
      } else {
        strip.setPixelColor(pos, 0);
      }
    }
  }

  strip.show();
  delay(1);
}
