/*
 * i2c_bitbang_esp8266.h
 *
 *  Created on: Jul 6, 2016
 *      Author: harry
 */

#ifndef SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_I2C_BITBANG_ESP8266_H_
#define SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_I2C_BITBANG_ESP8266_H_


#include <espressif/c_types.h>
#include "../cryptoauthlib.h"

#define MAX_I2C_BUSES   1      // we go for the default SDL/SDA

#define SDA0 	4
#define SCL0	5

typedef struct {
	uint32_t pin_sda[MAX_I2C_BUSES];
	uint32_t pin_scl[MAX_I2C_BUSES];
} I2CBuses;

extern I2CBuses i2c_buses_default;

extern uint32_t pin_sda;
extern uint32_t pin_scl;


/**
 * \brief Set I2C data and clock pin.
 *        Other functions will use these pins.
 *
 * \param[in] sda  definition of GPIO pin to be used as data pin
 * \param[in] scl  definition of GPIO pin to be used as clock pin
 */
void i2c_set_pin(uint32_t sda, uint32_t scl);

/**
 * \brief Configure GPIO pins for I2C clock and data as output.
 */
void i2c_enable(void);

/**
 * \brief Configure GPIO pins for I2C clock and data as input.
 */
void i2c_disable(void);


/**
 * \brief Send a START condition.
 */
void i2c_send_start(void);

/**
 * \brief Send an ACK or NACK (after receive).
 *
 * \param[in] ack  0: NACK, else: ACK
 */
void i2c_send_ack(uint8_t ack);

/**
 * \brief Send a STOP condition.
 */
void i2c_send_stop(void);

/**
 * \brief Send a Wake Token.
 */
void i2c_send_wake_token(void);

/**
 * \brief Send one byte.
 *
 * \param[in] i2c_byte  byte to write
 *
 * \return ATCA_STATUS
 */
ATCA_STATUS i2c_send_byte(uint8_t i2c_byte);

/**
 * \brief Send a number of bytes.
 *
 * \param[in] count  number of bytes to send
 * \param[in] data   pointer to buffer containing bytes to send
 *
 * \return ATCA_STATUS
 */
ATCA_STATUS i2c_send_bytes(uint8_t count, uint8_t *data);

/**
 * \brief Receive one byte (MSB first).
 *
 * \param[in] ack  0:NACK, else:ACK
 *
 * \return byte received
 */
uint8_t i2c_receive_one_byte(uint8_t ack);

/**
 * \brief Receive one byte and send ACK.
 *
 * \param[out] data  pointer to received byte
 */
void i2c_receive_byte(uint8_t *data);

/**
 * \brief Receive a number of bytes.
 *
 * \param[out] data   pointer to receive buffer
 * \param[in]  count  number of bytes to receive
 */
void i2c_receive_bytes(uint8_t count, uint8_t *data);


#endif /* SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_HAL_I2C_BITBANG_ESP8266_H_ */
