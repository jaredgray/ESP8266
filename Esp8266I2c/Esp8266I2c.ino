/*
 Name:		Esp8266I2c.ino
 Created:	5/23/2016 1:15:45 PM
 Author:	jared
*/
#include <ESP8266WiFi.h>
#include <Wire.h>

// the setup function runs once when you press reset or power the board
void setup() {
	Wire.begin(0, 2);
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
