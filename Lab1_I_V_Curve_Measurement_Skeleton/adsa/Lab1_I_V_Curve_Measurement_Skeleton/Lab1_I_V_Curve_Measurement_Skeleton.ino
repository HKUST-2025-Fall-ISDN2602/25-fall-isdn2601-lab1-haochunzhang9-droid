#include "Arduino.h"

#define Vin 12    // PWM output pin (GPIO12, safe)
#define Vout 32   // ADC input pin (GPIO32, ADC1 channel)

const float Vref = 3.3f;   // Reference voltage
const int ADC_MAX = 4095;  // 12-bit ADC maximum value

void setup() {
  Serial.begin(115200);
  pinMode(Vin, OUTPUT);
  pinMode(Vout, INPUT);

  // Configure ADC globally (works on all ADC1 pins)
  analogSetAttenuation(ADC_11db);  // allow measurement up to ~3.3 V

  delay(100);
  Serial.println("DutyCycle(%),Vout(V)");

  // Sweep duty cycle from 0 to 255
  for (int duty = 0; duty <= 255; duty++) {
    analogWrite(Vin, duty);   // duty 0–255
    delay(50);                // allow RC filter to settle

    // average multiple samples
    const int N = 8;
    int acc = 0;
    for (int i = 0; i < N; i++) {
      acc += analogRead(Vout);
      delay(2);
    }
    float adcValue = (float)acc / N;

    float dutyPercent = (float)duty * 100.0f / 255.0f;
    float vout = adcValue * Vref / ADC_MAX;

    Serial.print(dutyPercent, 2);
    Serial.print(",");
    Serial.println(vout, 3);
  }
}

void loop() {
  // Empty: Task 1 only needs one sweep
}
