#include "NodeCommands.h"
#include "user_config.h"
#include <SmingCore.h>

#include <security/libraries/cryptoauthlib/cryptoauthlib.h>



// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

NodeCommands cmd;

void serialCallBack(Stream& stream, char arrivedChar, unsigned short availableCharsCount) {

	if (arrivedChar == '\n') {
		char str[availableCharsCount];
		for (int i = 0; i < availableCharsCount; i++) {
			str[i] = stream.read();
			if (str[i] == '\r' || str[i] == '\n') {
				str[i] = '\0';
			}
		}
		if (!strcmp(str, "help")) {
			cmd.help();
		} else if (!strcmp(str, "client-provision")) {
			cmd.clientProvision();
		} else if (!strcmp(str, "client-build")) {
			cmd.clientBuild();
		} else if (!strcmp(str, "host-chain-verify")) {
			cmd.hostVerifyCertChain();
		} else if (!strcmp(str, "host-gen-chal")) {
			cmd.hostGenerateChallenge();
		} else if (!strcmp(str, "client-gen-resp")) {
			cmd.clientGenerateResponse();
		} else if (!strcmp(str, "host-verify-resp")) {
			cmd.hostVerifyResponse();
		} else if (!strcmp(str, "lockstat")) {
			cmd.lockstat();
		} else if (!strcmp(str, "lockcfg")) {
			cmd.lockcfg();
		} else if (!strcmp(str, "lockdata")) {
			cmd.lockdata();
		} else if (!strcmp(str, "info")) {
			cmd.info();
		} else if (!strcmp(str, "sernum")) {
			cmd.sernum();
		} else {
			Serial.println("unknown command");
		}
	}
}


ATCAIfaceCfg cfg_ateccx08a_i2c = {
		ATCA_I2C_IFACE, 	// active iface - how to interpret the union below
		ATECC508A, 			// explicit device type
		0xC0, 				// 8-bit slave address (I2C)
		1, 					// logical i2c bus number, 0-based - HAL will map this to a pin pair for SDA SCL
		200000, 			// baud rate i.e. I2C bus frequency - typically 400000
		800,				// wake_delay: microseconds of tWHI + tWLO which varies based on chip type
		20};				// the number of retries to attempt for receiving bytes

void setup_authlib() {
	Serial.println("Init Authentication Device\r\n");
	atcab_init(&cfg_ateccx08a_i2c);
}


void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial

	WifiAccessPoint.enable(false);
	WifiStation.enable(false);

	setup_authlib();

	delay(200);	// wait for the "garbadge" console output to be printed
	Serial.println();
	cmd.help();

	Serial.setCallback(serialCallBack);

}
