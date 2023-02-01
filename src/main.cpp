#include <stdio.h>
#include "pico/stdlib.h"
#include "at93c46.h"

int main()
{
	stdio_init_all();
	sleep_ms(2000);
	printf("starting\n");
	at93c46 eeprom = at93c46();
	//eeprom.set_mode(-1, true);

	eeprom.ew_enable();
	/*sleep_ms(1);
	eeprom.erase(2);
	sleep_ms(1);*/
	/*eeprom.write(0, 234);
	sleep_ms(10);
	eeprom.write(1, 235);
	sleep_ms(10);
	eeprom.write(2, 234);
	sleep_ms(10);
	eeprom.write(3, 236);
	sleep_ms(10);
	eeprom.write(4, 234);
	sleep_ms(10);
	eeprom.write(5, 237);
	sleep_ms(10);*/
	//sleep_ms(10);
	//eeprom.write_all(234);
	//sleep_ms(10);
	sleep_ms(10);
	//uint16_t data[5] = {1233, 43214, 1243, 3422, 3512};
	//eeprom.write_words(1, data, 5);
	sleep_ms(10);


	eeprom.ew_disable();
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
