/*
* Copyright © 2017 Adam Pyka <cavetronic@cavetronic.eu>
* This program is free software. It comes without any warranty, to the extent
* permitted by applicable law. You can redistribute it and/or modify it under
* the terms of the GNU GENERAL PUBLIC LICENSE, Version 3, as
* published by Free Software Foundation. See the LICENSE file for more details.
*/

#include "convert.h"

#include <stdlib.h>
#include <stdbool.h>

/* 
 * Helper function to check equality of two floats
 */
bool equality(float a, float b, float epsilon)
{
  if (fabs(a-b) < epsilon) return true;
  return false;
}

/*
 * Converts the acceleration data get from register (uint8_t LSB and MSB) to float in m/(s^2)
 */
float mma8451q_convert_to_ms2(uint8_t msb, uint8_t lsb){
	// This is a one-line method
	//return ((int16_t)((((uint16_t)msb << 8) | lsb) << 2) >> 2) * 1.0/4096 * 9.80665;
	
	// And this is a multiple line method for better understanding
	uint16_t raw = (uint16_t)msb << 8 | lsb; // raw 14-bit register value in U2
	int16_t amount = (int16_t)(raw << 2) >> 2;  // 16-bit signed value containting number of seeds
	float seed = 1.0/4096; // seed is 1 g/4096 ~ 0.25mg when 14-bit resolution in +-2g range
	float standard_gravity =  9.80665; // 1g is 9.80665 m/(s^2)
	
	return amount * seed * standard_gravity;
}

/*
 * Converts the acceleration data get from register (uint8_t LSB and MSB) to float in mm/(s^2)
 */
int mma8451q_convert_to_mms2(uint8_t msb, uint8_t lsb){
	// This is a one-line method
	//return (int32_t)((int64_t)((int16_t)((((uint16_t)msb << 8) | lsb) << 2) >> 2) * 980665 / 40960);
	
	// And this is a multiple line method for better understanding
	uint16_t raw = (uint16_t)msb << 8 | lsb; // raw 14-bit register value in U2
	int16_t amount = (int16_t)(raw << 2) >> 2;  // 16-bit signed value containting number of seeds
	
	// raw seed is 1 g/4096 ~ 0.25mg when 14-bit resolution in +-2g range, 1g is 9.80665 m/(s^2)
	return (int32_t)((int64_t)amount * 980665 / 40960);
}


int main(void){
	/*
	 * I know that that's not a very beautiful way of doing tests but... it's quick!
	 */	
	
	printf("Testing mma8451q_convert_to_ms2() by a datasheet values...\n"); 
	
	// Epsilon is 0.0024 m/(s^2) which was calculated in the basis of the accelerometer seed
	
	printf(" 19.610905798 \t is %f \t", mma8451q_convert_to_ms2(0b011111, 0b11111111));
	equality(mma8451q_convert_to_ms2(0b011111, 0b11111111), 19.610905798, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 19.608511597 \t is %f \t", mma8451q_convert_to_ms2(0b011111, 0b11111110));
	equality(mma8451q_convert_to_ms2(0b011111, 0b11111110), 19.608511597, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 0.002394202 \t is %f \t", mma8451q_convert_to_ms2(0b000000 , 0b000001));
	equality(mma8451q_convert_to_ms2(0b000000, 0b000001),  0.002394202, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 0.000000000 \t is %f \t", mma8451q_convert_to_ms2(0b000000, 0b000000));
	equality(mma8451q_convert_to_ms2(0b000000, 0b000000), 0.00000, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -0.002394202 \t is %f \t", mma8451q_convert_to_ms2(0b111111, 0b11111111));
	equality(mma8451q_convert_to_ms2(0b111111, 0b11111111), -0.002394202, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -19.610905798 \t is %f \t", mma8451q_convert_to_ms2(0b100000, 0b00000001));
	equality(mma8451q_convert_to_ms2(0b100000,  0b00000001), -19.610905798, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -19.6133 \t is %f \t", mma8451q_convert_to_ms2(0b100000, 0b00000000));
	equality(mma8451q_convert_to_ms2(0b100000, 0b00000000), -19.6133, 0.0024) ? printf("OK!\n") : printf("ERROR!!!\n");	
	
	
	printf("\nTesting mma8451q_convert_to_mms2() by a datasheet values...\n"); 
	
	printf(" 196109 \t is %d \t", mma8451q_convert_to_mms2(0b011111, 0b11111111));
	(mma8451q_convert_to_mms2(0b011111, 0b11111111) == 196109) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 196085 \t is %d \t", mma8451q_convert_to_mms2(0b011111, 0b11111110));
	(mma8451q_convert_to_mms2(0b011111, 0b11111110) == 196085) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 23      \t is %d      \t", mma8451q_convert_to_mms2(0b000000 , 0b000001));
	(mma8451q_convert_to_mms2(0b000000, 0b000001) == 23) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" 0       \t is %d      \t", mma8451q_convert_to_mms2(0b000000, 0b000000));
	(mma8451q_convert_to_mms2(0b000000, 0b000000) == 0) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -23     \t is %d      \t", mma8451q_convert_to_mms2(0b111111, 0b11111111));
	(mma8451q_convert_to_mms2(0b111111, 0b11111111) == -23) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -196109 \t is %d \t", mma8451q_convert_to_mms2(0b100000, 0b00000001));
	(mma8451q_convert_to_mms2(0b100000,  0b00000001) == -196109) ? printf("OK!\n") : printf("ERROR!!!\n");
	
	printf(" -196133 \t is %d \t", mma8451q_convert_to_mms2(0b100000, 0b00000000));
	(mma8451q_convert_to_mms2(0b100000, 0b00000000) == -196133) ? printf("OK!\n") : printf("ERROR!!!\n");	
	
	return 0;
}