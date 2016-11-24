#include "EEPromProvider.h"
#include <EEPROM.h>


EEPromProvider::EEPromProvider(HardwareSerial &serial)
{
	Serial = &serial;
}

EEPromProvider::EEPromProvider()
{
}

EEPromProvider::~EEPromProvider()
{
}



void EEPromProvider::init(size_t flash_size)
{
	EEPROM.begin(flash_size);
	reserved = EEPROM.read(0);
}


char * EEPromProvider::get_appversion()
{
	if (reserved != 1)
		return nullptr;

	char result[5];
	for (auto i = 1; i < 6; i++)
	{
		result[i - 1] = (char)EEPROM.read(i);
	}
	return result;
}

void EEPromProvider::set_appversion(char * version)
{
	for (auto i = 1; i < 6; i++)
	{
		EEPROM.write(i, (uint8_t)version[i - 1]);
	}
	if (reserved != 1)
	{
		EEPROM.write(0, 1);
	}
	EEPROM.commit();
}


char * EEPromProvider::get_string(int index, int length)
{
	char* result = new char[length];
	for (size_t i = index; i < length; i++)
	{
		result[i] = (char)EEPROM.read(i);
	}
	return result;
}

void EEPromProvider::set_string(char * value)
{
}


void Debug(char* value)
{
	if (Serial)
	{
		Serial.print(value);
	}
}
void DebugLine(char* value)
{
	if (Serial)
	{
		Serial.println(value);
	}
}

