/*
 * hal_esp8266_i2c_rtos.h
 *
 *  Created on: Jul 11, 2016
 *      Author: harry
 */

#ifndef SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_HAL_ESP8266_I2C_RTOS_H_
#define SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_HAL_ESP8266_I2C_RTOS_H_

#include <espressif/c_types.h>


//#define I2C_WRITE (uint8_t)0x00  //!< write command flag
//#define I2C_READ  (uint8_t)0x01   //!< read command flag

#define I2C_NO_STOP  0
#define I2C_STOP  1

/**
 * \brief This is the hal_data for ATCA HAL.
 */
typedef struct atcaI2Cmaster {
	uint32_t pin_sda;
	uint32_t pin_scl;
	int ref_ct;
	//! for conveniences during interface release phase
	int bus_index;
} ATCAI2CMaster_t;

typedef struct {
	uint32_t pin_sda;
	uint32_t pin_scl;
} I2CBuses;

#define MAX_I2C_BUSES 2

I2CBuses TWI_Bus[MAX_I2C_BUSES] = {{4, 5}, {0,2}};


#endif /* SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_HAL_ESP8266_I2C_RTOS_H_ */
