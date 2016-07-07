#include <security/test/cryptoauth_rng/include/user_config.h>
#include <SmingCore.h>

#include <security/libraries/cryptoauthlib/cryptoauthlib.h>


void setup() {

	uint8_t random_number[12];

//	system_init();

	atcab_init(&cfg_ateccx08a_i2c_default);

	atcab_random((uint8_t*)&random_number);

}


void init()
{

	setup();

	while (true) {
		;;
	}

}
