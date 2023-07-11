#include <Arduino.h>

HardwareSerial bigmeme(D0, D1);

void setup() {
    Serial.begin(57600);           // Set the baud rate for UART communication
    bigmeme.begin(57600);   // Initialize UART on Serial1


    // Serial1.println("Serial 1 ready");
    Serial.println("Serial 0 ready");
}

void loop() {
  while(bigmeme.available()){
    uint8_t data = bigmeme.read();
    Serial.print(data);
    Serial.print('\n');
  }
}

