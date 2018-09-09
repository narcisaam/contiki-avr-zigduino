#ifndef __SI7020_DRIVER_H__
#define __SI7020_DRIVER_H__

#include "lib/sensors.h"

extern const struct sensors_sensor si7020_sensor;
int read_humidity();

#define TEMPERATURE_SENSOR "Temperature"

#endif
