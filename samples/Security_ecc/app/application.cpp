#include <user_config.h>
#include <SmingCore.h>

#include "HexDump.h"


Timer procTimer;
bool state = true;
uint32_t startTime;

HexDump  hex_dump;


#include <uECC.h>

const struct uECC_Curve_t * curve = uECC_secp160r1();

void ecc_key1() {
	uint8_t private1[21];
	uint8_t public1[40];

	unsigned long a = millis();
	uECC_make_key(public1, private1, curve);
	unsigned long b = millis();

	Serial.print("Made key 1 in "); Serial.println(b-a);
}

void ecc_key2() {
	uint8_t private2[21];
	uint8_t public2[40];

	unsigned long a = millis();
	uECC_make_key(public2, private2, curve);
	unsigned long b = millis();
	Serial.print("Made key 2 in "); Serial.println(b-a);
}


void test_ecc() {
	  uint8_t private1[21];
	  uint8_t private2[21];

	  uint8_t public1[40];
	  uint8_t public2[40];

	  uint8_t secret1[20];
	  uint8_t secret2[20];

	  unsigned long a = millis();
	  uECC_make_key(public1, private1, curve);
	  unsigned long b = millis();

	  Serial.print("Made key 1 in "); Serial.println(b-a);
	  a = millis();
	  uECC_make_key(public2, private2, curve);
	  b = millis();
	  Serial.print("Made key 2 in "); Serial.println(b-a);

	  a = millis();
	  int r = uECC_shared_secret(public2, private1, secret1, curve);
	  b = millis();
	  Serial.print("Shared secret 1 in "); Serial.println(b-a);
	  if (!r) {
	    Serial.print("shared_secret() failed (1)\n");
	    return;
	  }

	  a = millis();
	  r = uECC_shared_secret(public1, private2, secret2, curve);
	  b = millis();
	  Serial.print("Shared secret 2 in "); Serial.println(b-a);
	  if (!r) {
	    Serial.print("shared_secret() failed (2)\n");
	    return;
	  }

	  if (memcmp(secret1, secret2, 20) != 0) {
	    Serial.print("Shared secrets are not identical!\n");
	  } else {
	    Serial.print("Shared secrets are identical\n");
	  }
}



void ecc_test_task(void *pvParameters)
{
	debugf("Test ecc");
	test_ecc();
	debugf("Test ecc done");
    vTaskDelete(NULL);
}

void rng_test_task(void *pvParameters)
{
	size_t size = 16;
	unsigned char *dest = new unsigned char[size]();
	debugf("RNG test");
	for (int i=0; i<20; i++) {
		os_get_random(dest, size);
		hex_dump.print(dest,size);
	}
	debugf("RNG_test done");
    vTaskDelete(NULL);
}


void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Allow debug output to serial

	//WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiStation.enable(false);
//	WifiAccessPoint.enable(false);

	startTime = millis();
	debugf("\n\nStart");
	debugf("\nInit RNG");
	uECC_set_rng(&os_get_random);
	debugf("Initialized in %d ms\n", startTime);

    xTaskCreate(rng_test_task, (const signed char*) "RNG_test_tesk", 256, NULL, 2, NULL);
    xTaskCreate(ecc_test_task, (const signed char*) "ecc_test_tesk", 256, NULL, 2, NULL);

}
