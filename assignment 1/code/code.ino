// a game with 3 lights corresponding to 3 buttons
// and another 3 lights representing one life each, and a button to reset the game
// finally, a toggle switch for turning game on/off (no need to program it)
// it will light up a pattern by blinking 6 times in different colors
// then player tries to repeat the pattern with the buttons
// if they get the pattern wrong, they lose a "life" until they have no life left
// when they get the full pattern right, the lights will dance!

// include neopixel library
#include <Adafruit_NeoPixel.h>

#define PIN      3
#define N_LEDS 12
#define patternLength 6
#define buttonLength 3

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// set LED colors to red, green, blue
const uint32_t ledColors[3] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255)};
// array of input button pins
const int buttonPins[buttonLength] = {13, 12, 11};

// declare arrays to store state for
// 1. the pattern (length 6 of integers)
int pattern[patternLength];
// 2. the player's input pattern so far (max length 6 of integers)
// 3. an integer starting at 2 of how many lives left
// 4. previous input button states (length 3 of booleans, defaulting to false at start)
int prevButtonState[buttonLength] = {LOW, LOW, LOW};

void setup() {
  // begin Neopixel strip
  strip.begin();
  // set button pin modes
  for (int i = 0; i < buttonLength; i += 1) {
    pinMode(buttonPins[i], INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  // check if reset button is pressed
  // if it is, call resetGame(), return

  // if pattern is empty, setup pattern
//  if (checkArrayEmpty(pattern)) {
//    setupPattern();
//  }

  // loop through all 3 input buttons and see if any were pressed
  // that weren't already pressed.
  // store new button states in button state array
  // light up corresponding light for each pressed button
  for (int i = 0; i < buttonLength; i += 1) {
    int buttonReading = digitalRead(buttonPins[i]);
    
    if (buttonReading != prevButtonState[i]) {
      Serial.print(buttonPins[i]);
      Serial.print(" is ");
      Serial.println(buttonReading);
    }

    prevButtonState[i] = buttonReading;
  }


  // if more than one button is pressed at a time,
  // treat that as incorrect and they lose a life

  // if none are pressed, return and do nothing

  // if one is pressed, add button number to input pattern array
  // and check whether the patterns match
  boolean matches = checkPlayerPattern();

  // if pattern does not match, check number of lives left
  // if more than 1 life, decrease number of lives
  // if only 1 life left, then play the death pattern
  // reset game

  // if pattern does match, check if pattern is complete (input pattern = length 6)
  // if yes, play celebration pattern, reset game
  // if no, continue

}

void clearNeopixel() {
  for (int i = 0; i < N_LEDS; i += 1) {
    strip.setPixelColor(i, 0);
  }

  strip.show();
}

boolean checkArrayEmpty(int arr[]) {
  boolean isEmpty = true;
  for (int i = 0; i < patternLength; i += 1) {
    if (arr[i] > 0) {
      isEmpty = false;
    }
  }

  return isEmpty;
}

// function to set up a pattern of random integers of 1, 2, 3
void setupPattern() {
  clearNeopixel();
  Serial.println("set up pattern");
  // loop through pattern array, randomly set number and light up that LED
  for (int i = 0; i < patternLength; i += 1) {
    int ledIndex = random(0, 3072) / 1024;
    pattern[i] = ledIndex;

    Serial.print(ledIndex);
    Serial.print("\t");
    Serial.println(pattern[i]);

    // light up LED
    strip.setPixelColor(ledIndex, ledColors[ledIndex]);
    strip.show();
    // wait 500ms, then turn it off
    delay(500);
    strip.setPixelColor(ledIndex, 0); // turn off
    strip.show();
    delay(250);
  }
}

// function to check if player input matches pattern
boolean checkPlayerPattern() {
  // loop through input pattern array, check if they match pattern array
}

// function to reset entire game
void resetGame() {
  // set pattern and input array to empty
  // set number of lives back to 2
  // set button states all back to false
}
