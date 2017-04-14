#pragma once
#include <stdlib.h>
#include <HardwareSerial.h>
class EEPromProvider
{
private:
	//int reserved;
	HardwareSerial* Serial;
public:
	EEPromProvider(HardwareSerial &serial);
	EEPromProvider();
	~EEPromProvider();
	void init(size_t flash_size);
	/*void set_appversion(char* version);
	char* get_appversion();*/

	String get_string(uint8_t key);
	bool set_string(uint8_t key, String value);
};

