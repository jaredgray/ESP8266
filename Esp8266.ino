//#include "ESP8266WiFi.h"
//#include <BlynkSimpleEsp8266.h>
//#include <SimpleTimer.h>
//#include <OneWire.h>
//#include <DallasTemperature.h>
//#include <stdlib.h>
//#define BLYNK_PRINT Serial
//#define ADC_INPUT A0
//#define LED_PORT 5
//#define TEMP_SENSOR_BUS 2 //4
//#define STRIP_PORT 12
//#define STRIP_LENGTH 60
//
//#define USEBLYNKSERVER 0
//
//
//char ThingAuth[] = "03102256200243b2ad5eda1be8e4fae9";
//char SSID[] = "VNET";
//char PASSWORD[] = "{7AE754BA-C379-4BAD-9801";
//
//SimpleTimer Interrupt;
//OneWire oneWire(TEMP_SENSOR_BUS);
//DallasTemperature TempSensor(&oneWire);
//
//void InterruptFunction()
//{
//	Serial.println("InterruptFunction");
//	int ADCValue;
//	double Temp;
//	char TempString[6];
//
//	ADCValue = analogRead(ADC_INPUT);
//
//	TempSensor.requestTemperatures();
//	Temp = TempSensor.getTempCByIndex(0);
//	dtostrf(Temp, 3, 2, TempString); 
//	TempString[4] = 'C';
//
//	
//	Serial.print(TempString);
//	
//#if USEBLYNKSERVER
//	Blynk.virtualWrite(0, 5 * ADCValue);
//	Blynk.virtualWrite(1, TempString);
//#endif
//
//}
//
//
//void setup()
//{
//	Serial.begin(115200);
//	Serial.println("Initializing..");
//#if USEBLYNKSERVER
//	Blynk.begin(ThingAuth, SSID, PASSWORD);
//#endif
//	Interrupt.setInterval(1000, InterruptFunction);
//	TempSensor.begin();
//	Serial.println("Initializing complete");
//}
//
//void loop()
//{
//#if USEBLYNKSERVER
//	Blynk.run();
//#endif
//	Interrupt.run();
//	delay(1000);
//}
//



//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
//#include <ESP8266WiFi.h>
//#include <BlynkSimpleEsp8266.h>
//
//// You should get Auth Token in the Blynk App.
//// Go to the Project Settings (nut icon).
//char auth[] = "03102256200243b2ad5eda1be8e4fae9";
//char SSID[] = "Serendipidy";
//char PASSWORD[] = "5417162080";
//
//void setup()
//{
//	Serial.begin(115200);
//	Blynk.begin(auth, "Serendipidy", "5417162080");
//}
//
//void loop()
//{
//	Blynk.run();
//}

#include <ESP8266WiFi.h>

const char* ssid = "Serendipity";
const char* password = "5417162080";

const char* host = "data.sparkfun.com";
const char* streamId = "....................";
const char* privateKey = "....................";

wl_status_t currentstatus;

void setup() {
	Serial.begin(115200);
	delay(10);

	// We start by connecting to a WiFi network

	Serial.println();
	Serial.println();
	Serial.print("Connecting to ");
	Serial.println(ssid);

	WiFi.begin(ssid, password, 0, 0, false);

	while (WL_CONNECTED != (currentstatus = WiFi.status())) {
		delay(500);
		Serial.print(".");
		Serial.println(currentstatus);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
	delay(5000);
	++value;

	Serial.print("connecting to ");
	Serial.println(host);

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return;
	}

	// We now create a URI for the request
	String url = "/input/";
	url += streamId;
	url += "?private_key=";
	url += privateKey;
	url += "&value=";
	url += value;

	Serial.print("Requesting URL: ");
	Serial.println(url);

	// This will send the request to the server
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");
	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println(">>> Client Timeout !");
			client.stop();
			return;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	while (client.available()) {
		String line = client.readStringUntil('\r');
		Serial.print(line);
	}

	Serial.println();
	Serial.println("closing connection");
}
