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
#define totalLives 3
#define resetPin 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

// set LED colors to yellow, green, red
const uint32_t ledColors[3] = {strip.Color(255, 128, 0), strip.Color(0, 255, 0), strip.Color(255, 0, 0)};
// array of input button pins
const int buttonPins[buttonLength] = {13, 12, 11};

// declare arrays to store state for
// # the pattern (length 6 of integers)
int pattern[patternLength];
// # the player's input pattern so far (max length 6 of integers)
int inputPattern[patternLength];
// # input pattern position
int patternPosition = 0;
// # an integer starting at 2 of how many lives left
int numLives = 3;
// # previous input button states (length 3 of booleans, defaulting to false at start)
int prevButtonState[buttonLength] = {LOW, LOW, LOW};
// previous reset button state
int prevResetState = LOW;

void setup() {
  // begin Neopixel strip
  strip.begin();
  // set button pin modes
  for (int i = 0; i < buttonLength; i += 1) {
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(resetPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // check if reset button is pressed
  // if it is, call resetGame(), return
  int resetReading = digitalRead(resetPin);
  if (resetReading != prevResetState) {
    if (resetReading == LOW) {
      // if reset button was pressed and released
      resetGame();
      Serial.println("reset");
    }
  }
  prevResetState = resetReading;

  //   if pattern is empty, setup pattern
  if (checkArrayEmpty(pattern)) {
    clearNeopixel();
    lightupLives();
    setupPattern();
  }

  // loop through all 3 input buttons and see if any were pressed
  // that weren't already pressed.
  // store new button states in button state array
  // light up corresponding light for each pressed button
  int buttonPressed = 0;
  for (int i = 0; i < buttonLength; i += 1) {
    int buttonReading = digitalRead(buttonPins[i]);

    if (buttonReading != prevButtonState[i]) {
      // if button state has changed, light up LED accordingly
      if (buttonReading == HIGH) {
        // if button is pressed, we want to turn on pixel
        strip.setPixelColor(i, ledColors[i]);
        // and remember it was pressed
        buttonPressed = i + 1;
      } else if (buttonReading == LOW) {
        // if button is released, turn off pixel
        strip.setPixelColor(i, 0);
      }
      strip.show();

      Serial.print(buttonPins[i]);
      Serial.print(" is ");
      Serial.println(buttonReading);
    }

    prevButtonState[i] = buttonReading;
  }


  // if more than one button is pressed at a time,
  // treat that as incorrect and they lose a life

  // if none are pressed, return and do nothing
  if (buttonPressed) {
    // if one is pressed, add button number to input pattern array
    // and check whether the patterns match
    inputPattern[patternPosition] = buttonPressed - 1;
    patternPosition += 1;
    boolean matches = checkPlayerPattern();

    if (!matches) {
      // if pattern is wrong, blink red
      for (int i = 0; i < 1; i += 1) {
        blinkRed();
        delay(250);
      }

      // then check number of lives left
      numLives -= 1;

      if (numLives == 0) {
        // if no more lives, blink red 3 times
        for (int i = 0; i < 3; i += 1) {
          blinkRed();
          delay(250);
        }
        resetGame();
      } else {
        lightupLives();
      }
    } else if (patternPosition == patternLength) {
      // if player has input all the pattern correctly
      // celebrate!
      for (int i = 0; i < 5; i += 1) {
        runPurple();
        // then reset game
        resetGame();
      }
    }
  }
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
  boolean isMatch = true;
  // loop through input pattern array, check if they match pattern array
  for (int i = 0; i < patternPosition; i += 1) {
    Serial.print(patternPosition);
    Serial.print("\t");
    Serial.print(pattern[i]);
    Serial.print(" and ");
    Serial.println(inputPattern[i]);
    if (pattern[i] != inputPattern[i]) {
      isMatch = false;
    }
  }

  return isMatch;
}

void lightupLives() {
  for (int i = 0; i < totalLives; i += 1) {
    if (i < numLives) {
      // light up that LED
      strip.setPixelColor(i + 4, strip.Color(0, 0, 255));
    } else {
      // clear LED
      strip.setPixelColor(i + 4, 0);
    }
    strip.show();
  }
}

void blinkRed() {
  // make all neopixels blink red
  for (int i = 0; i < N_LEDS; i += 1) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
  delay(250);
  for (int i = 0; i < N_LEDS; i += 1) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void runPurple() {
  for (int i = 0; i < N_LEDS + 4; i += 1) {
    strip.setPixelColor(i, strip.Color(128, 0, 128)); // Draw new pixel
    strip.setPixelColor(i - 4, 0); // Erase pixel a few steps back
    strip.show();
    delay(25);
  }
}

void resetArray(int arr[]) {
  for (int i = 0; i < patternLength; i += 1) {
    arr[i] = 0;
  }
}

// function to reset entire game
void resetGame() {
  // set pattern and input array to empty
  // set number of lives back to 2
  // set button states all back to false
  resetArray(pattern);
  resetArray(inputPattern);
  patternPosition = 0;
  numLives = 3;

  delay(1000);
}
