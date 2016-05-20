#include <user_config.h>
#include <SmingCore.h>

#include "HexDump.h"
#include "security/libraries/micro-ecc/uECC.h"


Timer procTimer;
bool state = true;
uint32_t startTime;

HexDump  hex_dump;

uint8_t private1[21];
uint8_t private2[21];

uint8_t public1[40];
uint8_t public2[40];

uint8_t secret1[20];
uint8_t secret2[20];

const struct uECC_Curve_t * curve = uECC_secp160r1();

extern "C" {

static int RNG(uint8_t *dest, unsigned size) {
       // Use the least-significant bits from the ADC for an unconnected pin (or connected to a source of
       // random noise). This can take a long time to generate random data if the result of analogRead(0)
       // doesn't change very frequently.
	uint8_t *dptr = dest;
	unsigned rsize = size;
	debugf("RNG (dest, %d)", size);
       while (size) {
              uint8_t val = 0;
              for (unsigned i = 0; i < 8; ++i) {
                     int init = system_adc_read();
                     int count = 0;
                     while (system_adc_read() == init) {
                           ++count;
                     }

                     if (count == 0) {
                           val = (val << 1) | (init & 0x01);
                     } else {
                           val = (val << 1) | (count & 0x01);
                     }
              }
              *dest = val;
              ++dest;
              --size;
       }
       hex_dump.print("RNG", dptr, rsize);
       // NOTE: it would be a good idea to hash the resulting random data using SHA-256 or similar.
       return 1;
}

static int OS_RNG(uint8_t *dest, unsigned size) {
	os_get_random(dest, size);
//    hex_dump.print("OS_RNG", dest, size);
	return 1;
}


}  // extern "C"



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
         } else {
        	 debugf("Shared secret1");
        	 hex_dump.print("Secret 1", secret1, 20);
         }

         a = millis();
         r = uECC_shared_secret(public1, private2, secret2, curve);
         b = millis();
         Serial.print("Shared secret 2 in "); Serial.println(b-a);
         if (!r) {
           Serial.print("shared_secret() failed (2)\n");
           return;
         } else {
        	 debugf("Shared secret2");
        	 hex_dump.print("Secret 2", secret2, 20);
         }

         if (memcmp(secret1, secret2, 20) != 0) {
           Serial.println("Shared secrets are not identical!");
         } else {
           Serial.println("Shared secrets are identical");
         }
}



void ecc_test_task(void *pvParameters)
{
    startTime = millis();
    debugf("Test ecc");
    test_ecc();
    debugf("Test ecc done in %d ms\n", startTime);
    vTaskDelete(NULL);
}

void rng_test_task(void *pvParameters)
{
       size_t size = 20;
       unsigned char *dest = new unsigned char[size]();
       unsigned char *dest2 = new unsigned char[size]();
       startTime = millis();
       debugf("RNG test");
       for (int i=0; i<16; i++) {
              RNG(dest, size);
              hex_dump.print("RNG", dest,size);
              //Serial.print(dest[i]);
              Serial.println("");
       }

       Serial.println("");
       Serial.println("");


       for (int i=0; i<16; i++) {
                     os_get_random(dest2, size);
                     hex_dump.print("os_get_random", dest2,size);
                     //Serial.print(dest2[i]);
                     Serial.println("");
              }

       debugf("RNG_test done in %d ms\n", startTime);
    vTaskDelete(NULL);
}


void init()
{
       Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
       Serial.systemDebugOutput(true); // Allow debug output to serial

       //WifiStation.config(WIFI_SSID, WIFI_PWD);
       WifiStation.enable(false);
//     WifiAccessPoint.enable(false);

       startTime = millis();
       debugf("\n\nStart");
       debugf("\nInit RNG");
//     uECC_set_rng(&os_get_random);
       uECC_set_rng(&RNG);
//       uECC_set_rng(&OS_RNG);
       debugf("Initialized in %d ms\n", startTime);

//    xTaskCreate(rng_test_task, (const signed char*) "RNG_test_tesk", 256, NULL, 2, NULL);
    xTaskCreate(ecc_test_task, (const signed char*) "ecc_test_task", 512, NULL, 2, NULL);

}

