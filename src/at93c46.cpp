#include <stdio.h>

#include "at93c46.h"
#include "hardware/spi.h"

#define AT93C46_CS 1
#define AT93C46_SK 2
#define AT93C46_DI 3
#define AT93C46_DO 4

at93c46::at93c46() 
	: _ew(false)
	, _long_mode(false)
{
	gpio_init(AT93C46_CS);
	gpio_set_dir(AT93C46_CS, GPIO_OUT);
	gpio_put(AT93C46_CS, 0);

	// Initialize SPI port at 2 MHz
	spi_init(spi0, 2*1000*1000);

	// Set SPI format
	spi_set_format( spi0,   // SPI instance
									16,      // Number of bits per transfer
									SPI_CPOL_0,      // Polarity (CPOL)
									SPI_CPHA_0,      // Phase (CPHA)
									SPI_MSB_FIRST);

	// Initialize SPI pins
	gpio_set_function(AT93C46_SK, GPIO_FUNC_SPI);
	gpio_set_function(AT93C46_DI, GPIO_FUNC_SPI);
	gpio_set_function(AT93C46_DO, GPIO_FUNC_SPI);

	
}

uint16_t at93c46::read(uint8_t address) {
	uint16_t buf[2];

	if (_long_mode) {
		buf[0] = 0b110000000|(address&0x3f);
	}
	else {
		buf[0] = 0b1100000000|(address&0x7f);
	}
	
	gpio_put(AT93C46_CS, 1);
	spi_write16_blocking(spi0, buf, 1);
	int num_bytes_read = spi_read16_blocking(spi0, 0, buf, 2);
	gpio_put(AT93C46_CS, 0);

	uint16_t data;

	if (_long_mode) {
		data = (buf[0]<<1)|(buf[1]>>15);
	}
	else {
		data = (buf[0]>>7)&0xff;
	}
	
	return data;
}

void at93c46::read_words (uint8_t address, uint16_t *data, uint8_t len) {
	uint16_t buf[100];
	uint8_t buf_len;

	if (_long_mode) {
		buf[0] = 0b110000000|(address&0x3f);
		buf_len = len+1;
	}
	else {
		buf[0] = 0b1100000000|(address&0x7f);
		buf_len = len/2+1;
	}

	if (buf_len > 100) {
		printf("ERROR: Reading too many bytes, increase the hardcoded buffer size.\n");
		return;
	}

	
	gpio_put(AT93C46_CS, 1);
	spi_write16_blocking(spi0, buf, 1);
	int num_bytes_read = spi_read16_blocking(spi0, 0, buf, buf_len);
	gpio_put(AT93C46_CS, 0);

	if (_long_mode) {
		for (int i = 0; i < len; i++) {
			data[i] = (buf[i]<<1)|(buf[i+1]>>15);
		}
	}
	else {
		for (int i = 0; i < len; i++) {
			if (i%2 == 0) {
				data[i] = (buf[i/2]>>7)&0xff;
			}
			else {
				data[i] = ((buf[i/2]<<1)|(buf[i/2+1]>>15))&0xff; 
			}
		}
	}
}

void at93c46::write(uint8_t address, uint16_t data) {
	uint16_t buf[2];

	if (_long_mode) {
		buf[0] = 0b101000000|(address&0x3f);
		buf[1] = data;
	}
	else {
		buf[0] = 0b10;
		buf[1] = ((0b10000000|address)<<8)|(data&0xff);
	}
	
	gpio_put(AT93C46_CS, 1);
	int num_bytes_written = spi_write16_blocking(spi0, buf, 2);
	gpio_put(AT93C46_CS, 0);
	sleep_ms(5);
}

void at93c46::write_words(uint8_t address, uint16_t *data, uint8_t len) {
	ew_enable();
	for (int i = 0; i < len; i++) {
		write(address+i, data[i]);
	}
	ew_disable();
}

void at93c46::ew_enable() {
	uint16_t buf[1];

	if (_ew) {
		return;
	}

	if (_long_mode) {
		buf[0] = 0b100110000;
	}
	else {
		buf[0] = 0b1001100000;
	}
	
	gpio_put(AT93C46_CS, 1);
	spi_write16_blocking(spi0, buf, 1);
	gpio_put(AT93C46_CS, 0);
	_ew = true;
}

void at93c46::ew_disable() {
	uint16_t buf[1];

	if (!_ew) {
		return;
	}

	if (_long_mode) {
		buf[0] = 0b100000000;
	}
	else {
		buf[0] = 0b1000000000;
	}
	
	gpio_put(AT93C46_CS, 1);
	spi_write16_blocking(spi0, buf, 1);
	gpio_put(AT93C46_CS, 0);
	_ew = false;
}

void at93c46::erase(uint8_t address) {
	uint16_t buf[4];

	if (_long_mode) {
		buf[0] = 0b111000000|(address&0x3f);
	}
	else {
		buf[0] = 0b1110000000|(address&0x7f);
	}
	
	gpio_put(AT93C46_CS, 1);
	int num_bytes_written = spi_write16_blocking(spi0, buf, 1);
	gpio_put(AT93C46_CS, 0);

}

void at93c46::erase_all() {
	uint16_t buf[1];

	if (_long_mode) {
		buf[0] = 0b100100000;
	}
	else {
		buf[0] = 0b1001000000;
	}
	
	gpio_put(AT93C46_CS, 1);
	spi_write16_blocking(spi0, buf, 1);
	gpio_put(AT93C46_CS, 0);

}

void at93c46::write_all(uint16_t data) {
	uint16_t buf[2];

	if (_long_mode) {
		buf[0] = 0b100010000;
		buf[1] = data;
	}
	else {
		buf[0] = 0b10;
		buf[1] = (0b00100000<<8)|(data&0xff);
	}

	
	gpio_put(AT93C46_CS, 1);
	int num_bytes_written = spi_write16_blocking(spi0, buf, 2);
	gpio_put(AT93C46_CS, 0);
	sleep_ms(5);
}

void at93c46::set_mode(int pin, bool long_mode) {
	_long_mode = long_mode;

	if (pin >= 0) {
		gpio_init(pin);
		gpio_set_dir(pin, GPIO_OUT);
		gpio_put(pin, long_mode);
	}
}