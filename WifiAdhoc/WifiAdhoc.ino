/*
 Name:		WifiAdhoc.ino
 Created:	2/25/2017 8:36:39 PM
 Author:	jared
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

//IPAddress local_IP(192, 168, 4, 22);
//IPAddress gateway(192, 168, 4, 9);
//IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(80);

String SSID_LOCAL("IOT-NET");
String SSID_Local_Pass;

String SSID_Wan("VNET");
String SSID_Wan_Pass("7AE754BA-C379-4BAD-9801");

void setup()
{
	Serial.begin(115200);
	Serial.println();

	Serial.println("Setting up...");

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
	wl_status_t currentstatus;
	//IPAddress ip(192, 168, 1, 200); // where xx is the desired IP Address
	//IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
	//Serial.print(F("Setting static ip to : "));
	//Serial.println(ip);
	//IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
	//WiFi.config(ip, gateway, subnet);

	WiFi.mode(WIFI_STA);
	WiFi.begin(SSID_Wan.c_str(), SSID_Wan_Pass.c_str());

	while (WL_CONNECTED != (currentstatus = WiFi.status())) {
		delay(500);
		Serial.print(".");
		Serial.println(currentstatus);
	}

	Serial.println(WiFi.localIP());
}
void SetupWebServer()
{
	server.on("/", HandleRootRequest);
	server.begin();
	Serial.println("HTTP server started");
}

void HandleRootRequest() 
{
	server.send(200, "text/html", "<h1>You are connected</h1>");
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
}