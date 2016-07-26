/*
 * hal_eps8266_i2c_RTOS.c
 *
 *  Created on: Jul 13, 2016
 *      Author: harry
 */


#include <espressif/c_types.h>
#include <espressif/esp_libc.h>
#include <espressif/esp_misc.h>

#include <security/libraries/cryptoauthlib/hal/hal_esp8266_i2c_RTOS.h>

#include <sming/services/ArduinoESP_Wire/twi.h>
#include <sming/services/ArduinoESP_Wire/esp8266_peri.h>

#include "atca_hal.h"
#include "../atca_status.h"
#include "../atca_iface.h"

#include "../trace/trace.h"


uint32_t hal_sda, hal_scl;

#define I2C_DATA_LOW() 	(GPES = (1 << hal_sda)) //Enable SDA (becomes output and since GPO is 0 for the pin, it will pull the line low)
#define I2C_DATA_HIGH() (GPEC = (1 << hal_sda)) //Disable SDA (becomes input and since it has pullup it will go high)



ATCA_STATUS hal_i2c_init( void *hal, ATCAIfaceCfg *cfg) {

	ATCAHAL_t *phal = (ATCAHAL_t*)hal;
    uint8_t bus = cfg->atcaunion.atcai2c.bus - 1;

    hal_sda = TWI_Bus[bus].pin_sda;
    hal_scl = TWI_Bus[bus].pin_scl;

    os_printf("\nhal_i2c_init -> twi_init(sda %d, scl %d); \n", hal_sda, hal_scl);

	twi_init(hal_sda, hal_scl);

    uint32_t freq = cfg->atcaunion.atcai2c.baud;

    twi_setClock(freq);

    return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_post_init(ATCAIface iface) {
    // TODO: define what to do here
    return ATCA_SUCCESS;
}


ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength) {

	ATCAIfaceCfg *cfg = atgetifacecfg(iface);
	uint8_t address = cfg->atcaunion.atcai2c.slave_address;
	txdata[0] = 0x03;   //!< Word Address Value = Command
	txlength++;         //!< count Word Address byte towards txlength

	tracef("hal_i2c_send (addr)", &address, 1);
	tracef("hal_i2c_send (data)", txdata, txlength);

	uint8_t result = twi_writeTo(address>>1, txdata, txlength, I2C_STOP);

	switch (result) {
	case 2:
		os_printf("\n\t hal_i2c_send - received NACK on transmit of address\r\n");
		return ATCA_TX_FAIL;
	case 3:
		os_printf("\n\t hal_i2c_send - received NACK on transmit of data\r\n");
		return ATCA_TX_FAIL;
	case 4:
		os_printf("\n\t hal_i2c_send - line busy\r\n");
		return ATCA_TX_FAIL;
	default:
		break;
	}
    return ATCA_SUCCESS;
};

ATCA_STATUS hal_i2c_receive( ATCAIface iface, uint8_t *rxdata, uint16_t *rxlength) {

	ATCAIfaceCfg *cfg = atgetifacecfg(iface);

	ATCA_STATUS status = ATCA_RX_TIMEOUT;

	uint8_t bus = cfg->atcaunion.atcai2c.bus - 1;
	uint8_t address = cfg->atcaunion.atcai2c.slave_address;
	int retries = cfg->rx_retries;

	char *buf = (char *)rxlength;
	uint16_t len = (uint16_t)buf[0];

	while (retries-- > 0 && status != ATCA_SUCCESS) {
		//! Address the device and indicate that bytes are to be read
		status = twi_readFrom(address>>1, rxdata, len, I2C_STOP);
	}

	switch (status) {
	case 2:
		os_printf("\n\t hal_i2c_receive - received NACK on transmit of address\r\n");
		return ATCA_TX_TIMEOUT;
	case 4:
		os_printf("\n\t hal_i2c_receive - line busy\r\n");
		return ATCA_RX_NO_RESPONSE;
	default:
		break;
	}
	tracef("hal_i2c_receive ", rxdata, len);
    return ATCA_SUCCESS;
};



ATCA_STATUS hal_i2c_wake(ATCAIface iface) {

	ATCAIfaceCfg *cfg = atgetifacecfg(iface);

    uint8_t bus = cfg->atcaunion.atcai2c.bus - 1;

    uint32_t sda = TWI_Bus[bus].pin_sda;

	ATCA_STATUS status = ATCA_WAKE_FAILED;

	uint8_t response[4] = { 0x00, 0x00, 0x00, 0x00 };
	uint8_t expected_response[4] = { 0x04, 0x11, 0x33, 0x43 };
	uint16_t rxlength = sizeof(response);

	// we try max 3 times
	uint8_t retries = 3;
	while (retries-- && status) {
		//! Generate Wake Token
		I2C_DATA_LOW();
		os_delay_us(80);
		I2C_DATA_HIGH();

		//! Wait tWHI + tWLO
		atca_delay_us(cfg->wake_delay);

		//! Receive Wake Response
		status = hal_i2c_receive(iface, response, (uint16_t *)&rxlength);
	}

	if (status == ATCA_SUCCESS) {
		//! Compare response with expected_response
		if (memcmp(response, expected_response, 4) != 0) {
			os_printf("\n-->hal_esp8266_i2c_rtos.c\tATCA_STATUS hal_i2c_wake(ATCAIface iface) FAILED %d\n", status);
			os_printf("\n-->hal_esp8266_i2c_rtos.c\tretries %d\n", retries);
			status = ATCA_WAKE_FAILED;
		}
	}
	return status;
};


ATCA_STATUS hal_i2c_idle(ATCAIface iface) {

	ATCAIfaceCfg *cfg = atgetifacecfg(iface);

    uint8_t bus = cfg->atcaunion.atcai2c.bus - 1;

    uint8_t address = cfg->atcaunion.atcai2c.slave_address;

	unsigned char txdata[] = {0x02};
	uint16_t txlength = 1;

	//! Address the device and indicate that bytes are to be written
	ATCA_STATUS status = twi_writeTo(address>>1, txdata, txlength, I2C_STOP);
	if (status == ATCA_SUCCESS) {
		return ATCA_SUCCESS;
	}
	return ATCA_TX_TIMEOUT;
};


ATCA_STATUS hal_i2c_sleep(ATCAIface iface)
{
	ATCAIfaceCfg *cfg = atgetifacecfg(iface);

    uint8_t bus = cfg->atcaunion.atcai2c.bus - 1;

    uint8_t address = cfg->atcaunion.atcai2c.slave_address;

	unsigned char txdata[] = {0x01};
	uint16_t txlength = 1;

	//! Address the device and indicate that bytes are to be written
	ATCA_STATUS status = twi_writeTo(address>>1, txdata, txlength, I2C_STOP);
	if (status == ATCA_SUCCESS) {
		return ATCA_SUCCESS;
	}
	return ATCA_TX_TIMEOUT;
}


ATCA_STATUS hal_i2c_release(void *hal_data ) {

	// if the use count for this bus has gone to 0 references, disable it.  protect against an unbracketed release
	twi_stop();

	return ATCA_SUCCESS;
}


ATCA_STATUS hal_i2c_discover_buses(int i2c_buses[], int max_buses) {
	// TODO  -> implement when required
	return ATCA_SUCCESS;
};

ATCA_STATUS hal_i2c_discover_devices(int busNum, ATCAIfaceCfg *cfg, int *found ) {
	// TODO  -> implement when required
	return ATCA_SUCCESS;
};
