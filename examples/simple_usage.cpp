#include <stdio.h>
#include "pico/stdlib.h"
#include "at93c46.h"

#define AT93C46_CS 1
#define AT93C46_SK 2
#define AT93C46_DI 3
#define AT93C46_DO 4
#define SPI_HARDWARE spi0

int main()
{
	stdio_init_all();
	sleep_ms(2000);
	printf("starting\n");
	AT93C46 eeprom = AT93C46(AT93C46_CS, AT93C46_SK, AT93C46_DI, AT93C46_DO, SPI_HARDWARE);
	//eeprom.set_mode(-1, true);

	sleep_ms(10);
	uint16_t data[5] = {23, 243, 123, 244, 132};
	eeprom.write_words(1, data, 5);
	sleep_ms(10);

	while (true) {
		uint16_t buf[6];
		eeprom.read_words(0, buf, 6);
		for (int i = 0; i < 6; i++) {
			printf("%u ", buf[i]);
		}
		printf("\n");

		sleep_ms(1000);
	}
}
