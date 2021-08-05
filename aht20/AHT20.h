#ifndef __AHT20_H__
#define __AHT20_H__

#include "driver.h"
#include "device.h"
#include "i2c-dev.h"

#define AHT20_POLLING_MS 300

class AHT20 : public Device {
	private:
		enum {
			 s_detect, s_read, s_trigger, s_error, s_wait
		} state;
		TickType_t tickcnt, polling_tickcnt, measurement_tickcnt;

		float t = 0, h = 0;

	public:
		// constructor
		AHT20(int bus_ch, int dev_addr);

		// override
		void init(void);
		void process(Driver *drv);
		int prop_count(void);
		bool prop_name(int index, char *name);
		bool prop_unit(int index, char *unit);
		bool prop_attr(int index, char *attr);
		bool prop_read(int index, char *value);
		bool prop_write(int index, char *value);

		// method
		float read(uint8_t type);
};

#endif
