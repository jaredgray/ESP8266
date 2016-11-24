#include "ESP8266WiFi.h"
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdlib.h>
#define BLYNK_PRINT Serial
//#define BLYNK_LOG1(p1) { Serial.println(p1); }
//#define BLYNK_LOG2(p1,p2) { Serial.println(p1 + p2); }
#define ADC_INPUT A0
#define LED_PORT 5
#define TEMP_SENSOR_BUS 2 //4
#define STRIP_PORT 12
#define STRIP_LENGTH 60

#define USEBLYNKSERVER 1


char ThingAuth[] = "03102256200243b2ad5eda1be8e4fae9";
char* SSID = "Serendipity";
char* PASSWORD = "5417162080";
//char SSID[] = "VNET";
//char PASSWORD[] = "{7AE754BA-C379-4BAD-9801";

SimpleTimer Interrupt;
OneWire oneWire(TEMP_SENSOR_BUS);
DallasTemperature TempSensor(&oneWire);

void InterruptFunction()
{
	Serial.println("InterruptFunction");
	int ADCValue;
	double Temp;
	char TempString[6];

	ADCValue = analogRead(ADC_INPUT);

	TempSensor.requestTemperatures();
	Temp = TempSensor.getTempCByIndex(0);
	dtostrf(Temp, 3, 2, TempString);
	TempString[4] = 'C';


	Serial.print(TempString);

#if USEBLYNKSERVER
	Blynk.virtualWrite(0, 5 * ADCValue);
	Blynk.virtualWrite(1, TempString);
#endif

}


void setup()
{
	Serial.begin(115200);
	Serial.println("Initializing..");
#if USEBLYNKSERVER
	Blynk.begin(ThingAuth, SSID, PASSWORD);
#endif
	Interrupt.setInterval(1000, InterruptFunction);
	TempSensor.begin();
	Serial.println("Initializing complete");
}

void loop()
{
#if USEBLYNKSERVER
	Blynk.run();
#endif
	Interrupt.run();
	delay(1000);
}

