/*
*  This sketch demonstrates how to scan WiFi networks.
*  The API is almost the same as with the WiFi Shield library,
*  the most obvious difference being the different file you need to include:
*/
#include <EEPROM.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266HTTPClient.h>
#include "ESP8266WiFi.h"
#include <ArduinoOTA.h>
//#include "EEPromProvider.h"
//#include <SoftwareSerial\src\SoftwareSerial.h>

const int DELAY_SECONDS = 30;
const char* VNET_SSID = "VNET";
const char* VNET_PASSWORD = "7AE754BA-C379-4BAD-9801";
const char* Version = "1.0.0";
//EEPromProvider flash;
 
void setup() {
	Serial.begin(115200);
	//flash.init(512);

	Serial.println();

	Serial.print("Initial version of wifiscan");
	//Serial.print("New and improved! version of wifiscan");
	
	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	connect_to_access_point(VNET_SSID, VNET_PASSWORD);
	// check for ota updates
	check_for_updates();

	Serial.println("Setup done");
	
}

void connect_to_access_point(const char* ssid, const char* password)
{
	wl_status_t currentstatus;
	IPAddress ip(192, 168, 1, 200); // where xx is the desired IP Address
	IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
	Serial.print(F("Setting static ip to : "));
	Serial.println(ip);
	IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
	WiFi.config(ip, gateway, subnet);

	WiFi.begin(ssid, password, 0, 0, false);

	while (WL_CONNECTED != (currentstatus = WiFi.status())) {
		delay(500);
		Serial.print(".");
		Serial.println(currentstatus);
	}
}

void check_for_updates()
{
	Serial.println("attempting to check update from server");

	auto uri = String("/api/Update/Esp8266WifiScan/1.0.0");

	// http://192.168.1.253:43863/api/Update/Esp8266WifiScan/1.0.0
	auto ret = ESPhttpUpdate.update("192.168.1.253", 43863, uri, Version);

	if (ret == HTTP_UPDATE_FAILED)
		Failed();
	else if (ret == HTTP_UPDATE_NO_UPDATES)
		NoUpdate();
	else if (ret == HTTP_UPDATE_OK)
		UpdateSuccessful();
	else
		Serial.println("no matching response.");
	
	Serial.println("finished check update from server");
}

void Failed()
{
	Serial.println("OTA Update failed.");
}
void NoUpdate()
{
	Serial.println("no OTA Update.");
}
void UpdateSuccessful()
{
	Serial.println("OTA Update ok."); // may not called we reboot the ESP
}

void saveDeviceID(const char* id)
{

	Serial.println("do eeprom:");
	Serial.println(id);
	String newIDs = String(id);
	int addr = 0;
	EEPROM.write(addr, newIDs.toInt());
	addr = addr + 1;
	if (addr == 512)
	{
		addr = 0;
		EEPROM.commit();
	}

}

void loop() {
	Serial.println("scan start");

	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	if (n == 0)
		Serial.println("no networks found");
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.print(")");
			Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
			delay(10);
		}
	}
	Serial.println("");

	// Wait a bit before scanning again
	delay(1000 * DELAY_SECONDS);
}
