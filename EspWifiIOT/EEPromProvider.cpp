#include "EEPromProvider.h"
#include <EEPROM.h>
#include <unordered_map>
#include <vector>

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

const int RESERVED_STORAGE = 72;
std::unordered_map<uint8_t, std::vector<uint8_t>> string_map;
uint8_t pointer = 0;

void EEPromProvider::init(size_t flash_size)
{
	EEPROM.begin(flash_size);
	//reserved = EEPROM.read(0);

	// 72 bytes will be taken up with string metadata
	// which allocates 24 strings avalable in memory.
	// 1 byte is taken up with metadata about state.
	// (this could be done a whole lot better if we
	// split the bytes and used 2 bytes per slot
	// instead of 3, + we would get another 7 strings
	// for free, though the limit of each string
	// would go down to 31) but it'll
	// work for now #quickanddirty
	// this is offset from 256 bytes to allow the 
	// data storage to be sored below it. the reason
	// for this is to allow as much length in the pointer
	// index of the data 
	for (size_t i = 256; i < 256 + 24; i += 3)
	{
		auto key = EEPROM.read(i);
		auto index = EEPROM.read(i + 1);
		auto length = EEPROM.read(i + 2);

		std::vector<uint8_t> value = { index, length };

		Serial->print(key);
		Serial->print(", ");
		Serial->print(index);
		Serial->print(", ");
		Serial->print(length);

		string_map.emplace(key, value);

		// increment our pointer so we know
		// where next to write in the data
		if (index > 0 && index > pointer)
			pointer = index;
	}


}


String EEPromProvider::get_string(uint8_t key)
{
	auto finder = string_map.find(key);
	if (string_map.end() != finder)
	{
		auto length = string_map[key][1];
		size_t index = string_map[key][0];
		String result;

		for (size_t i = index; i < length; i++)
		{
			result += char(EEPROM.read(i));
		}

		return result;
	}
	else
	{
		return "";
	}
}

bool EEPromProvider::set_string(uint8_t key, String value)
{
	auto finder = string_map.find(key);
	auto length = (uint8_t)value.length();
	uint8_t index = 0;

	if (string_map.end() != finder)
	{
		auto storageinfo = finder->second;
		
		index = storageinfo[0];
		auto prevlen = storageinfo[1];
		// if the length of the new string is greater than what we had
		// stored previously, refuse to write the new value
		if (length > prevlen)
		{
			return false;
		}

		for (size_t i = 0; i < value.length(); i++)
		{
			auto letter = value.charAt(i);
		}

		EEPROM.put(index, value);

	}
	else
	{
		EEPROM.put(pointer, value);
		index = pointer;
		pointer += length;
	}
	EEPROM.commit();
	std::vector<uint8_t> candidate = { index, length };
	auto emplacement = string_map.emplace(key, candidate);
	if (!emplacement.second)
	{
		string_map[key][0] = index;
		string_map[key][1] = length;
	}

	return true;
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

