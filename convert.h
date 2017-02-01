/*
* Copyright © 2017 Adam Pyka <cavetronic@cavetronic.eu>
* This program is free software. It comes without any warranty, to the extent
* permitted by applicable law. You can redistribute it and/or modify it under
* the terms of the GNU GENERAL PUBLIC LICENSE, Version 3, as
* published by Free Software Foundation. See the LICENSE file for more details.
*/

#ifndef CONVERT_H
#define CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include <math.h>

/*
 * Converts the acceleration data get from register (uint8_t LSB and MSB) to float in m/(s^2)
 */
float mma8451q_convert_to_ms2(uint8_t msb, uint8_t lsb);

/*
 * Converts the acceleration data get from register (uint8_t LSB and MSB) to float in mm/(s^2)
 */
int mma8451q_convert_to_mms2(uint8_t msb, uint8_t lsb);

#ifdef __cplusplus
}
#endif

#endif /* CONVERT_H */
