/*
 * hal_esp8266_timer.c
 *
 *  Created on: Jul 6, 2016
 *      Author: harry
 */

#include "atca_hal.h"

#include "espressif/esp_misc.h"

/** \defgroup hal_ Hardware abstraction layer (hal_)
 *
 * \brief
 * These methods define the hardware abstraction layer for communicating with a CryptoAuth device
 *
   @{ */

void atca_delay_us(uint32_t delay)
{
	// use espressif os delay
	os_delay_us(delay);
}

/** \brief This function delays for a number of tens of microseconds.
 *
 * \param[in] delay number of 0.01 milliseconds to delay
 */
void atca_delay_10us(uint32_t delay)
{
	// use espressif os delay
	os_delay_us( delay * 10 );
}

/** \brief This function delays for a number of milliseconds.
 *
 *         You can override this function if you like to do
 *         something else in your system while delaying.
 * \param[in] delay number of milliseconds to delay
 */

void atca_delay_ms(uint32_t delay)
{
	// use espressif os delay
	os_delay_us(delay * 1000);
}



