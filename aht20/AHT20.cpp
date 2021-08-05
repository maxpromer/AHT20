#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "kidbright32.h"
#include "AHT20.h"

AHT20::AHT20(int bus_ch, int dev_addr) {
	channel = bus_ch;
	address = dev_addr;
	polling_ms = AHT20_POLLING_MS;
}

void AHT20::init(void) {
	t = 0;
	h = 0;
	state = s_detect;
}

int AHT20::prop_count(void) {
	return 0;
}

bool AHT20::prop_name(int index, char *name) {
	// not supported
	return false;
}

bool AHT20::prop_unit(int index, char *unit) {
	// not supported
	return false;
}

bool AHT20::prop_attr(int index, char *attr) {
	// not supported
	return false;
}

bool AHT20::prop_read(int index, char *value) {
	// not supported
	return false;
}

bool AHT20::prop_write(int index, char *value) {
	// not supported
	return false;
}

void AHT20::process(Driver *drv) {
	I2CDev *i2c = (I2CDev *)drv;

	switch (state) {
		case s_detect:
			if (i2c->detect(channel, address) == ESP_OK) {
				if (i2c->write(channel, address, (uint8_t*)"\xBE", 1) == ESP_OK) {
					error = false;
					initialized = true;

					state = s_trigger;
				} else {
					state = s_error;
				}
			} else {
				state = s_error;
			}
			break;
		case s_trigger:
			if (is_tickcnt_elapsed(polling_tickcnt, polling_ms)) {
				if (i2c->write(channel, address, (uint8_t*)"\xAC\x33\x00", 3) == ESP_OK) {
					measurement_tickcnt = get_tickcnt();
					state = s_read;
				} else {
					state = s_error;
				}
			}
			break;
		case s_read:
			if (is_tickcnt_elapsed(measurement_tickcnt, 200)) {
				uint8_t data[6];
				if (i2c->read(channel, address, NULL, 0, data, 6) == ESP_OK) {
					if ((data[0] & 0x80) == 0) {
						h = (uint32_t)(((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (((uint32_t)data[3] & 0xF0) >> 4)) / 1048576.0 * 100.0;
						t = (uint32_t)((((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | (uint32_t)data[5]) / 1048576.0 * 200.0 - 50.0;

						polling_tickcnt = get_tickcnt();
						state = s_trigger;
					} else {
						state = s_error;
					}
				} else {
					state = s_error;
				}
			}
			break;

		case s_error:
			// set error flag
			error = true;
			// clear initialized flag
			initialized = false;
			// init temperature
			t = -999;
			h = -999;
			// get current tickcnt
			tickcnt = get_tickcnt();
			// goto wait and retry with detect state
			state = s_wait;
			break;

		case s_wait:
			// wait polling_ms timeout
			if (is_tickcnt_elapsed(tickcnt, polling_ms)) {
				state = s_detect;
			}
			break;
	}
}

float AHT20::read(uint8_t type) {
	return type == 0 ? t : h;
}
