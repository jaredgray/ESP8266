#pragma once
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
class IOTConnectionManager
{
public:
	IOTConnectionManager();
	~IOTConnectionManager();

	void Initialize();
};

