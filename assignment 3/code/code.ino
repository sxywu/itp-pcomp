// read brightness on each photoresistor
// send the change in brightness to Serial

// 5 photoresistors, 1 button for resetting
#define numSensors 2
#define buttonPin 2

// array of sensor pins
const int sensorPins[numSensors] = {17, 21};
// array of default/normalized sensor readings
int sensorDefaults[numSensors];
// remember reset button state (HIGH bc pullup resistor)
int prevButtonState = HIGH;

void setup() {
  // set button state
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState != prevButtonState && buttonState == LOW) {
    // if reset button is pressed, re-read sensor values
    resetSensorValues();
  }
  prevButtonState = buttonState;

  // then read sensors
}

// for each sensor, read 20 values and remember average
void resetSensorValues() {
  for (int i = 0; i < 20; i += 1) {
    // read sensor states
    for (int sensor = 0; sensor < numSensors; sensor += 1) {
      int sensorPin = sensorPins[sensor];
      int brightness = analogRead(sensorPin);

      if (i == 0) {
        // if first reading, remember it in sensorDefaults
        sensorDefaults[sensor] = brightness;
      } else {
        // else set it to average of prev and current value
        sensorDefaults[sensor] = (sensorDefaults[sensor] + brightness) / 2;
      }

//      Serial.print(brightness);
//      Serial.print("\t");
//      Serial.println(sensorDefaults[sensor]);
    }
    delay(100);
  }

//  Serial.print(sensorDefaults[0]);
//  Serial.print(", ");
//  Serial.println(sensorDefaults[1]);
}
