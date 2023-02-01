// Motivation https://github.com/0xJoey/Arduino_93C46

#ifndef __AT93C46_H__
#define __AT93C46_H__

#include "pico/stdlib.h"

class at93c46 {
public:
	at93c46 ();
	
	/// @brief Set the memory organization, whether 128 words of 8 bits or 64 words of 16 bits
	/// @param pin The pin connected to EEPROM ORG pin, if less than 0, then assume that it was set in hardware and just to setup software correctly
	/// @param long_mode Whether a word is 16 bits or not
	void set_mode (int pin, bool long_mode);

	// Instruction set
	uint16_t read (uint8_t address);
	void write(uint8_t address, uint16_t data);
	void write_all(uint16_t data);
	void erase(uint8_t address);
	void erase_all();
	void ew_enable();
	void ew_disable();

	// Additional functionality
	void read_words (uint8_t address, uint16_t *data, uint8_t len);
	void write_words (uint8_t address, uint16_t *data, uint8_t len);

private:
	bool _ew;
	bool _long_mode;
};

#endif