#include <security/test/cryptoauth_rng/include/user_config.h>

//#include <security/libraries/i2c_driver/i2c_twi.h>

#include <sming/services/ArduinoESP_Wire/twi.h>



#include <SmingCore.h>

#include <security/libraries/cryptoauthlib/cryptoauthlib.h>
#include "HeXDump.h"

#define LED_PIN 2 // GPIO2

#define SDA	4
#define	SCL 5

Timer procTimer;
bool state = true;


HexDump dump = HexDump();

ATCAIfaceCfg cfg_ateccx08a_i2c = {ATCA_I2C_IFACE, ATECC508A, 0xC0, 1, 400000, 800, 20};
//ATCAIfaceCfg cfg_ateccx08a_i2c = {ATCA_I2C_IFACE, ATECC508A, 0x60, 1, 400000, 800, 20};

ATCAIface if_ateccx08a_i2c = newATCAIface(&cfg_ateccx08a_i2c);  // constructor


void blink()
{
	digitalWrite(LED_PIN, state);
	state = !state;
}


void setup() {

	debugf("setup i2c");

	/*
	debugf("i2c_master_gpio_init");
	i2c_gpio_init(SDA, SCL);

	debugf("i2c i2c_master_init");
	i2c_init();

	debugf("i2c twi_write_byte");
	i2c_writeByte(0x42);
	*/

	debugf("===>  hal_i2c_init()");
	hal_i2c_init(null, &cfg_ateccx08a_i2c);
	debugf("===>  hal_i2c_init()");

	debugf("===>  ATCA_STATUS hal_i2c_post_init(ATCAIface iface)");
	hal_i2c_post_init(if_ateccx08a_i2c);
	debugf("===>  ATCA_STATUS hal_i2c_post_init(ATCAIface iface)");

	debugf("===>  ATCA_STATUS hal_i2c_wake(ATCAIface iface)");
	hal_i2c_wake(if_ateccx08a_i2c);
	debugf("===>  ATCA_STATUS hal_i2c_wake(ATCAIface iface)");

	/*
	debugf("===>  ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength)");
	uint8_t txdata[4] = { 0x00, 0x01, 0x21, 0x42};
	uint8_t txlength = 4;
	hal_i2c_send(if_ateccx08a_i2c, txdata, txlength);
	debugf("===>  ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength)");
	*/

	debugf("===>  ATCA_STATUS hal_i2c_idle(ATCAIface iface)");
	hal_i2c_idle(if_ateccx08a_i2c);
	debugf("===>  ATCA_STATUS hal_i2c_idle(ATCAIface iface)");

	debugf("===>  ATCA_STATUS hal_i2c_wake(ATCAIface iface)");
//	hal_i2c_wake(if_ateccx08a_i2c);
	debugf("===>  ATCA_STATUS hal_i2c_wake(ATCAIface iface)");

	debugf("===>  ATCA_STATUS hal_i2c_sleep(ATCAIface iface)");
	hal_i2c_sleep(if_ateccx08a_i2c);
	debugf("===>  ATCA_STATUS hal_i2c_sleep(ATCAIface iface)");

	debugf("===>  ATCA_STATUS hal_i2c_release(void *hal_data)");
	hal_i2c_release(NULL);
	debugf("===>  ATCA_STATUS hal_i2c_release(void *hal_data)");

}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(true); // Allow debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(false);

	pinMode(LED_PIN, OUTPUT);
	procTimer.initializeMs(1000, blink).start();

	os_delay_us(100);

	debugf("\r\nCryptoAuthLib Test start \r\n");

	setup();

	debugf("CryptoAuthLib Test done \r\n");



}
