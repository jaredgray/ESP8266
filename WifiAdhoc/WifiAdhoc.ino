/*
 Name:		WifiAdhoc.ino
 Created:	2/25/2017 8:36:39 PM
 Author:	jared

 Comment:	this is a comment for something to push
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <unordered_map>
#include <vector>
#include "EEPromProvider.h"
#include <EEPROM.h>

//IPAddress local_IP(192, 168, 4, 22);
//IPAddress gateway(192, 168, 4, 9);
//IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);
unsigned long last_millis = 0;
int millis_elapsed = 0;
int MILLIS_WAIT = 1000 * 0;


const String DEVICE_HOSTNAME = String("SmartLight_IOTDeviceSetup");
String SSID_LOCAL(DEVICE_HOSTNAME);
String SSID_Local_Pass;

String SSID_Wan("VNET");
String SSID_Wan_Pass("7AE754BA-C379-4BAD-9801");
bool DoConnectToWan;

void setup()
{
	Serial.begin(115200);
	Serial.println();

	//testUnorderedMap();

	Serial.println("Setting up...");



	delay(500);

	WiFi.mode(WIFI_AP_STA);
	WiFi.enableAP(true);
	WiFi.enableSTA(true);

	WiFi.hostname(DEVICE_HOSTNAME);

	ConnectSoftAp();
	//ConnectWan();

	SetupWebServer();
}

String GetMacAddress()
{
	byte mac[6];
	WiFi.macAddress(mac);
	return macToStr(mac);
}

void ConnectSoftAp()
{
	SSID_Local_Pass = GetMacAddress();

	/*Serial.print("Setting soft-AP configuration ... ");
	Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");*/


	Serial.print("Setting soft-AP with password ");
	Serial.print(SSID_Local_Pass);
	Serial.print(" ... ");
	Serial.println(WiFi.softAP(SSID_LOCAL.c_str(), SSID_Local_Pass.c_str()) ? "Ready" : "Failed!");

	Serial.print("Soft-AP IP address = ");
	Serial.println(WiFi.softAPIP());
}
void ConnectWan()
{
	if (!DoConnectToWan)
		return;
	DoConnectToWan = false;
	Serial.println("ConnectWan called");
	//WiFi.enableAP(false);
	//WiFi.softAPdisconnect();

	// here we will want to save our connection info - for now we're just hard coded.

	wl_status_t currentstatus;
	//IPAddress ip(192, 168, 1, 200); // where xx is the desired IP Address
	//IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
	//Serial.print(F("Setting static ip to : "));
	//Serial.println(ip);
	//IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
	//WiFi.config(ip, gateway, subnet);

	//WiFi.mode(WIFI_STA);
	WiFi.begin(SSID_Wan.c_str(), SSID_Wan_Pass.c_str());

	Serial.println("Connecting to WAN........");
	while (WL_CONNECTED != (currentstatus = WiFi.status())) {
		delay(1000);
		Serial.print(".");
		Serial.println(currentstatus);
	}
	Serial.println("Connected to WAN");

	//WiFi.enableAP(true);
	auto localip = WiFi.localIP();
	auto response = String("{\"success\": true, \"pingback\": true, \"\": " + localip.toString() + " }");

	server.send(200, "application/json", response);
	Serial.println(response);

	response = String("{\"success\": true, \"localIp\": \"") + localip.toString() + "\" }";
	Serial.println(response);
}
void SetupWebServer()
{
	server.on("/", HandleRootRequest);
	server.on("/sayhello", HandleSayHello);
	server.begin();
	Serial.println("HTTP server started");
}

void HandleRootRequest()
{
	StaticJsonBuffer<200> newBuffer;
	auto payload = server.arg("plain");
	JsonObject& root = newBuffer.parseObject(payload);

	if (!root.success())
	{
		Serial.println("Invalid payload:");
		Serial.println(payload);
		server.send(400, "application/json", String("{\"Error\": \"Request was in an unexpected format.\", \"Received\": \"") + payload + "\" }");
	}
	else
	{
		if (root.containsKey("ssid") && root.containsKey("key"))
		{
			const char* ssid = root["ssid"];
			const char* key = root["key"];

			Serial.println("Request to switch networks successful...");
			Serial.println(ssid);
			Serial.println(key);

			//auto response = String("{\"success\": true, \"localIp\": \"") + localip.toString() + "\" }";


			DoConnectToWan = true;
		}
		else
		{
			server.send(400, "application/json", "{\"Error\": \"Request json did not have expected properties.\"}");
		}
	}

}
void HandleSayHello()
{
	server.send(200, "text/html", "<html><body><h1>Wellllll... Heloooooooow</h1></body></html>");
}

String macToStr(const uint8_t* mac)
{
	String result;
	for (int i = 0; i < 6; ++i) {
		result += String(mac[i], 16);
		if (i < 5)
			result += ':';
	}
	return result;
}

void loop()
{
	server.handleClient();
	if (DoConnectToWan)
	{
		if (millis_elapsed > MILLIS_WAIT)
		{
			ConnectWan();
			last_millis = 0;
			millis_elapsed = 0;
		}
		else
		{
			if (last_millis > 0)
			{
				millis_elapsed += (millis() - last_millis);
			}
			last_millis = millis();
		}
	}
}

/*                   --         tests below          --                        */
EEPromProvider provider(Serial);
//std::unordered_map<uint8_t, std::vector<uint8_t>> string_map;
void testUnorderedMap()
{
	provider.init(512);
	provider.dump();
	provider.clear();
	provider.set_string(1, "Here is my stored string.");
	provider.set_string(2, "And this is my second stored string.");

	auto entry = provider.get_string(1);
	if (entry.length() > 0)
	{
		Serial.print("1 - Found entry: ");
		Serial.println(entry);
	}

	entry = provider.get_string(2);
	if (entry.length() > 0)
	{
		Serial.print("2 - Found entry: ");
		Serial.println(entry);
	}
	provider.dump();
	provider.clear();
}