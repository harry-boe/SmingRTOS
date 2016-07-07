/*
 * i2c_bitbang_esp8266.c
 *
 *  Created on: Jul 6, 2016
 *      Author: harry
 */


#include "i2c_bitbang_esp8266.h"

#include "../i2c_driver/i2c_master.h"

I2CBuses i2c_buses_default = {
	{ SDA0 },
	{ SCL0 }
};

uint32_t pin_sda = (uint32_t) i2c_buses_default.pin_sda;
uint32_t pin_scl = (uint32_t) i2c_buses_default.pin_scl;


void i2c_set_pin(uint32_t sda, uint32_t scl)
{
	// we ignore the sda/scl
	pin_sda = sda;
	pin_scl = scl;

	i2c_master_gpio_init();

}

void i2c_enable(void)
{
	i2c_master_gpio_init();
}

void i2c_disable(void)
{
	// todo check how to disable
}

void i2c_send_start(void) {
	i2c_master_start();
}

void i2c_send_ack(uint8_t ack) {
	i2c_master_setAck(ack);
}

void i2c_send_stop(void) {
	i2c_master_stop();
}

void i2c_send_wake_token(void) {
	//	I2C_DATA_LOW();
	I2C_MASTER_SDA_HIGH_SCL_LOW();
	// delay_us(80);
	i2c_master_wait(80);
	// I2C_DATA_HIGH();
	I2C_MASTER_SDA_HIGH_SCL_HIGH();
};

ATCA_STATUS i2c_send_byte(uint8_t i2c_byte) {

	ATCA_STATUS status = ATCA_TX_TIMEOUT;
	// we wont get any feedback from i2c_master
	i2c_master_writeByte(i2c_byte);
	// so we always return success
	status = ATCA_SUCCESS;

	return status;
}

ATCA_STATUS i2c_send_bytes(uint8_t count, uint8_t *data) {
	ATCA_STATUS status = ATCA_TX_TIMEOUT;

	uint8_t i;

	for (i = 0; i < count; i++) {
		status = i2c_send_byte(data[i]);
		if (status != ATCA_SUCCESS) {
			if (i > 0)
				status = ATCA_TX_FAIL;
			break;
		}
	}
	return status;
}

uint8_t i2c_receive_one_byte(uint8_t ack) {

	uint8_t data = i2c_master_readByte();
	//
	i2c_master_send_ack();

	return data;
}

void i2c_receive_byte(uint8_t *data) {
		*data = i2c_receive_one_byte(1);
}


void i2c_receive_bytes(uint8_t count, uint8_t *data)
{
	while (--count)
		*data++ = i2c_receive_one_byte(1);
	*data = i2c_receive_one_byte(0);

	i2c_send_stop();
}

