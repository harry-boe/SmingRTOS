/*
 * commands.c
 *
 *  Created on: Jul 22, 2016
 *      Author: harry
 */

#include <SmingCore.h>
#include <security/libraries/cryptoauthlib/cryptoauthlib.h>

#include "NodeCommands.h"
#include "provision.h"
#include "node_auth.h"
#include "cert_def_1_signer.h"
#include "cert_def_2_device.h"

#include <sming/services/HexDump/HexDump.h>
extern HexDump dump;

NodeCommands::NodeCommands()
{
	debugf("NodeCommands Instantiating");
}

NodeCommands::~NodeCommands()
{
}


void NodeCommands::help() {
	printf("\r\nUsage:\r\n");

	printf("client-provision  - Configure and load certificate data onto ATECC device.\r\n");
	printf("client-build      - Read certificate data off ATECC device and rebuild full signer and device certificates.\r\n");
	printf("host-chain-verify - Verify the certificate chain from the client.\r\n");
	printf("host-gen-chal     - Generate challenge for the client.\r\n");
	printf("client-gen-resp   - Generate response to challenge from host.\r\n");
	printf("host-verify-resp  - Verify the client response to the challenge.\r\n");
	printf("Utility functions:\r\n");
	printf("lockstat - zone lock status\r\n");
	printf("lockcfg  - lock config zone - WARNING set to read only can't be undone\r\n");
	printf("lockdata - lock data and OTP zones - WARNING set to read only can't be undone\r\n");
	printf("info     - get the chip revision\r\n");
	printf("sernum   - get the chip serial number\r\n");
	printf("random   - generate random number\r\n");
	printf("cert-tmpl- show cert template data\r\n");
	printf("dev-tmpl - how device template data\r\n");

	printf("\r\n");
}


void NodeCommands::clientProvision() {

	int result = 0;

	printf("Client Provisioning\r\n");
	result = client_provision();
	printf("Result [%d]\r\n", result);

}

void NodeCommands::clientBuild() {

	int result = 0;

	printf("Client Build\r\n");
	result = client_rebuild_certs();
	printf("Result [%d]\r\n", result);

}

void NodeCommands::hostVerifyCertChain() {

	int result = 0;

	printf("Host Verify Chain\r\n");
	result = host_verify_cert_chain();
	printf("Result [%d]\r\n", result);

}

void NodeCommands::clientGenerateResponse() {

	int result = 0;

	printf("Client Generate Response\r\n");
	result = client_generate_response();
	printf("Result [%d]\r\n", result);

}

void NodeCommands::hostGenerateChallenge() {

	int result = 0;

	printf("Host Generate Challenge\r\n");
	result = host_generate_challenge();
	printf("Result [%d]\r\n", result);

}

void NodeCommands::hostVerifyResponse() {

	int result = 0;

	printf("Host Verify Response\r\n");
	result = host_verify_response();
	printf("Result [%d]\r\n", result);

}




/** \brief getinfo returns the revision bytes from the chip.  These bytes identify
 * the exact revision of the silicon.
 */
void NodeCommands::info() {

	uint8_t revision[4];
	atcab_info( revision );

	printf("Chip Revision : [");
	this->printbuf(revision, 4);
	printf("]\r\n");
}

/** \brief getsernum returns the serial number bytes from the chip.  These bytes identify
 * the exact instance of a device - as expected, all serial numbers are unique across devices.
 */
void NodeCommands::sernum()
{
	int status;
	uint8_t serial[9] = {0};

	status = atcab_read_serial_number(serial);

	printf("Chip Serial Number : [");
	this->printbuf(serial, 9);
	printf("]\r\n");
}

void NodeCommands::random(void) {
	int status;
	uint8_t random_number[32] = {0};

	atcab_random((uint8_t*)&random_number);

	printf("Generate Random Number : [");
	this->printbuf(random_number, 32);
	printf("]\r\n");

}

void NodeCommands::ctempl(void) {

	printf("CA Template  : [");
	dump.print((unsigned char*)g_cert_template_1_signer, sizeof(g_cert_template_1_signer));
	printf("]\r\n");

}

void NodeCommands::dtempl(void) {

	printf("Device Template  : [");
	dump.print((unsigned char*)g_cert_template_2_device, sizeof(g_cert_template_1_signer));
	printf("]\r\n");

}

/** \brief  lockstatus queries the lock status of configuration and data zones
 *  and prints the status of the zones to the console
 *  \return ATCA_STATUS
 */

void NodeCommands::lockstat(void)
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

	printf("lockstatus = %d\r\n", status);
}

/** \brief lock_config_zone will lock the configuration zone of the CryptoAuth device
 * \returns the lock response from the chip
 */
void NodeCommands::lockcfg(void)
{
	uint8_t lock_response;
	atcab_lock_config_zone( &lock_response );

	printf("lock_config_zone = %d\r\n", lock_response);
}

/** \brief lock_data_zone will lock the data zone of the CryptoAuth device
 * \returns the lock response from the chip
 */
void NodeCommands::lockdata(void)
{
	uint8_t lock_response;
	atcab_lock_data_zone( &lock_response );

	printf("lock_data_zone = %d\r\n", lock_response);
}


void NodeCommands::printbuf(uint8_t * buf, int len) {

	uint8_t idx = 0;
	// print address

	// Show the hex codes
	for( int i = 0; i < len; i++ )
	{
	   if ( i % 8 == 0 ) os_printf(" ");
	   if (i < len) {
		      idx = (buf[i] & 0Xf0) >>4;
		      os_printf("%c", _oc[idx]);
		      idx = buf[i] & 0X0f;
		      os_printf("%c ", _oc[idx]);
	   }
	}
}



