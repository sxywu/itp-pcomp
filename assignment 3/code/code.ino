// read brightness on each photoresistor
// send the change in brightness to Serial

// 5 photoresistors, 1 button for resetting
#define numSensors 2
#define buttonPin 2
// how much to increase/decrease by
#define margin 2

// array of sensor pins
const int sensorPins[numSensors] = {17, 21};
// array of default/normalized sensor readings
int sensorLows[numSensors];
// array of flower animation percentages
int animationPercents[numSensors] = {0, 0};
// remember reset button state (HIGH bc pullup resistor)
int prevButtonState = HIGH;

void setup() {
  // set button state
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);

  resetSensorValues();
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState != prevButtonState && buttonState == LOW) {
    // if reset button is pressed, re-read sensor values
    resetSensorValues();
  }
  prevButtonState = buttonState;

  calcAnimationPercents();

  // send animation values
  for (int sensor = 0; sensor < numSensors; sensor += 1) {
    Serial.print(animationPercents[sensor]);
    if (sensor < numSensors - 1) {
      Serial.print(",");
    } else {
      Serial.println();
    }
  }

  delay(50);
}

// read each sensor and calculate what % of animation
// the corresponding flower should be at
void calcAnimationPercents() {
  for (int sensor = 0; sensor < numSensors; sensor += 1) {
    // 1. read sensor
    int sensorPin = sensorPins[sensor];
    int brightness = analogRead(sensorPin);
    
    // 2. map brightness to number between 0 and 100
    brightness = map(brightness, sensorLows[sensor] + 50, 1000, 0, 100);
    brightness = constrain(brightness, 0, 100);
    
    // 3. compare brightness to current animation percent
    if (animationPercents[sensor] < brightness) {
      // current reading is bigger so add 5% and constrain to 100%
      animationPercents[sensor] = min(animationPercents[sensor] + margin, 100);
    } else if (animationPercents[sensor] > brightness) {
      animationPercents[sensor] = max(0, animationPercents[sensor] - margin);
    }

    delay(10);
  }
}

// for each sensor, read 20 values and remember average
void resetSensorValues() {
  for (int i = 0; i < 20; i += 1) {
    // read sensor states
    for (int sensor = 0; sensor < numSensors; sensor += 1) {
      int sensorPin = sensorPins[sensor];
      int brightness = analogRead(sensorPin);

      if (i == 0) {
        // if first reading, remember it in sensorLows
        sensorLows[sensor] = brightness;
      } else {
        // else set it to average of prev and current value
        sensorLows[sensor] = (sensorLows[sensor] + brightness) / 2;
      }

//      Serial.print(brightness);
//      Serial.print("\t");
//      Serial.println(sensorLows[sensor]);
    }
    delay(100);
  }

//  Serial.print(sensorLows[0]);
//  Serial.print(", ");
//  Serial.println(sensorLows[1]);
}
