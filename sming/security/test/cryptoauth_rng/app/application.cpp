#include <security/test/cryptoauth_rng/include/user_config.h>
#include <SmingCore.h>

#include <security/libraries/cryptoauthlib/cryptoauthlib.h>
#include "HeXDump.h"

#define LED_PIN 2 // GPIO2

Timer procTimer;
bool state = true;


HexDump dump = HexDump();


ATCAIfaceCfg cfg_ateccx08a_i2c = {ATCA_I2C_IFACE, ATECC508A, 0xC0, 1, 400000, 800, 20};


void blink()
{
	digitalWrite(LED_PIN, state);
	state = !state;
}


void setup() {

	debugf("setup RNG");

	os_printf("\nuint8_t random_number[12];\n");

	uint8_t random_number[12] = {0};

//	system_init();

	debugf("atcab_init(&cfg_ateccx08a_i2c);");
	atcab_init(&cfg_ateccx08a_i2c);
	debugf(".. done\r\n");
	dump.print((unsigned char*)&cfg_ateccx08a_i2c, sizeof(cfg_ateccx08a_i2c));


	int status;
	if ( (status = atcab_wakeup()) != ATCA_SUCCESS ) {
		debugf("setup RNG failed\r\n");
		return;
	}

	debugf("atcab_random((uint8_t*)&random_number);");
	atcab_random((uint8_t*)&random_number);
	debugf(".. done\r\n");

	dump.print(random_number, 12);

	debugf("setup RNG done\r\n");

}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(true); // Allow debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(false);

//	pinMode(LED_PIN, OUTPUT);
//	procTimer.initializeMs(1000, blink).start();

	os_delay_us(100);

	debugf("\r\nCryptoAuthLib Test start \r\n");

	setup();

	debugf("CryptoAuthLib Test done \r\n");



}
