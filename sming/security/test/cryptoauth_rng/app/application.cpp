#include <security/test/cryptoauth_rng/include/user_config.h>
#include <SmingCore.h>

#include <security/libraries/cryptoauthlib/cryptoauthlib.h>
#include "HeXDump.h"

#define LED_PIN 2 // GPIO2

Timer procTimer;
bool state = true;


HexDump dump = HexDump();


ATCAIfaceCfg cfg_ateccx08a_i2c = {ATCA_I2C_IFACE, ATECC508A, 0xC0, 1, 400000, 800, 20};


ATCA_STATUS lockstatus(void)
{
	ATCA_STATUS status;
	bool dataIsLocked=0xff, cfgIsLocked=0xff;

	if ( (status = atcab_is_locked( LOCK_ZONE_CONFIG, &cfgIsLocked )) != ATCA_SUCCESS )
		printf("can't read cfg lock\r\n");
	else if ( (status = atcab_is_locked( LOCK_ZONE_DATA, &dataIsLocked )) != ATCA_SUCCESS )
		printf("can't read data lock\r\n");

	if ( status == ATCA_SUCCESS ) {
        printf("Config Zone Lock: %s\r\n", cfgIsLocked == 0x01 ? "locked" : "unlocked");
        printf("Data Zone Lock  : %s\r\n", dataIsLocked == 0x01 ? "locked" :"unlocked" );
    }

	return status;
}

/** \brief lock_config_zone will lock the configuration zone of the CryptoAuth device
 * \returns the lock response from the chip
 */
static uint8_t lock_config_zone(void)
{
	uint8_t lock_response;
	atcab_lock_config_zone( &lock_response );

	return lock_response;
}

/** \brief lock_data_zone will lock the data zone of the CryptoAuth device
 * \returns the lock response from the chip
 */
static uint8_t lock_data_zone(void)
{
	uint8_t lock_response;
	atcab_lock_data_zone( &lock_response );

	return lock_response;
}

/** \brief getinfo returns the revision bytes from the chip.  These bytes identify
 * the exact revision of the silicon.
 */

static ATCA_STATUS getinfo( uint8_t *revision )
{
	return atcab_info( revision );
}



void getSerialNo() {

	os_printf("\n#################################\n");
	os_printf("\n====>	getSerialNo()\n");

	int status;
	uint8_t serial[9] = {0};

	status = atcab_read_serial_number(serial);

	dump.print(serial, 9);
	os_printf("\ngetSerialNo()\n");
	os_printf("\n#################################\n");

}

void getRandom() {
	os_printf("\n#################################\n");
	debugf("\n====>	atcab_random((uint8_t*)&random_number);");

	int status;
	uint8_t random_number[32] = {0};

	atcab_random((uint8_t*)&random_number);

	dump.print(random_number, 32);

	debugf("\n====>	atcab_random((uint8_t*)&random_number);");
	os_printf("\n#################################\n");
}

void setup() {

	debugf("setup RNG");

//	debugf("\natcab_init(&cfg_ateccx08a_i2c);");
	atcab_init(&cfg_ateccx08a_i2c);
//	debugf(".. done\r\n");
//	dump.print((unsigned char*)&cfg_ateccx08a_i2c, sizeof(cfg_ateccx08a_i2c));


	int status;
	if ( (status = atcab_wakeup()) != ATCA_SUCCESS ) {
		debugf("setup atcab_wakeup failed\r\n");
		return;
	} else {

	/*
	debugf("*******************************");
	lock_config_zone();
	debugf("*******************************");
	lock_data_zone();
	*/
	debugf("*******************************");
//	lockstatus();
	/*
	debugf("*******************************");
	uint8_t *revision ;
	getinfo(revision);
	debugf("Revision %d", revision);
	debugf("*******************************");
	*/
	debugf("*************** getSerialNo ****************");
	getSerialNo();
	debugf("*************** getRandom ****************");
	getRandom();

	}
}


void init()
{

	Serial.begin(115200); // 115200 by default
	Serial.systemDebugOutput(true); // Allow debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(false);

	os_delay_us(100);

	debugf("\r\nCryptoAuthLib Test start \r\n");

	setup();

	debugf("CryptoAuthLib Test done \r\n");

}
