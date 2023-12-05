#include <stdio.h>
#include <string.h>

// Define the pins to which the TP224 sensor's output pins are connected
const int touchPin1 = 15;  // Connect TP224 OUT1 to Arduino digital pin 2
const int touchPin2 = 3;  // Connect TP224 OUT2 to Arduino digital pin 3
const int touchPin3 = 4;  // Connect TP224 OUT3 to Arduino digital pin 4
const int touchPin4 = 5;  // Connect TP224 OUT4 to Arduino digital pin 5

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set the TP224 sensor pins as INPUT_PULLUP
  pinMode(touchPin1, INPUT_PULLUP);
  pinMode(touchPin2, INPUT_PULLUP);
  pinMode(touchPin3, INPUT_PULLUP);
  pinMode(touchPin4, INPUT_PULLUP);
}

void loop() {
  // Read the state of each touch sensor pin
  int touchState1 = digitalRead(touchPin1);
  int touchState2 = digitalRead(touchPin2);
  int touchState3 = digitalRead(touchPin3);
  int touchState4 = digitalRead(touchPin4);

  // Print the state of each touch sensor to the serial monitor


  /*Serial.print("Touch Sensor 1: ");
  Serial.println(touchState1);
  Serial.print("Touch Sensor 2: ");
  Serial.println(touchState2);
  Serial.print("Touch Sensor 3: ");
  Serial.println(touchState3);
  Serial.print("Touch Sensor 4: ");
  Serial.println(touchState4); */

  if (touchState1 == 1) {
    Serial.println("Your brat");
  }
  // Add a small delay to avoid rapid serial output
  delay(500);
}