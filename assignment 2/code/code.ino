// used JSON example:
// https://github.com/arduino-libraries/Arduino_JSON/blob/master/examples/JSONArray/JSONArray.ino

// 4 buttons, 6 LEDs
// - 2 buttons for lunch & dinner
// - 2 buttons to toggle days
// - 4 LEDs for 2 days worth of lunch & dinner
// - 2 LEDs to indicate which day

// include neopixel library
#include <Adafruit_NeoPixel.h>
#include <Arduino_JSON.h>

#define LED_PIN      2
#define N_LEDS 21
#define numButtons 2
#define numDays 7

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

// option of lunch & dinner
const int allButtons[numButtons] = {21, 17};
const uint32_t lightColors[numButtons + 1] = { strip.Color(128, 0, 128), strip.Color(255, 255, 0),  strip.Color(0, 128, 255)};

int prevButtonState[numButtons] = {HIGH, HIGH};

void setup() {
  // begin Neopixel strip
  strip.begin();
  strip.setBrightness(128);

  // set button pin modes
  for (int i = 0; i < numButtons; i += 1) {
    pinMode(allButtons[i], INPUT_PULLUP);
  }

  Serial.begin(9600);
  while (!Serial);

  Serial.println("ready");
}

void loop() {
  if (Serial.available()) {
    JSONVar lightsArray = JSON.parse(Serial.readString());

    // light up LEDs based on data
    for (int pixel = 0; pixel < N_LEDS; pixel += 1) {
      int lightUp = lightsArray[pixel];
      uint32_t color = 0;
      if (lightUp) {
        int row = pixel / 7;
        color = lightColors[row];
      }

      strip.setPixelColor(pixel, color);
      strip.show();
    }
  } else {
    readButtons();
  }
}

void readButtons() {
  // see if any are pressed
  for (int button = 0; button < numButtons; button += 1) {
    int buttonReading = digitalRead(allButtons[button]);

    if (buttonReading == LOW && buttonReading != prevButtonState[button]) {
      // if button state has changed & it's been pressed
      // (LOW because of PULLUP)
      Serial.println(button + 1);
    }

    // remember button state
    prevButtonState[button] = buttonReading;
  }
}
