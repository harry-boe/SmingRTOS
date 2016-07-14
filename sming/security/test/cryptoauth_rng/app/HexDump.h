/*
 * HexDump.h
 *
 *  Created on: Jul 8, 2016
 *      Author: harry-boe
 */

#ifndef SMING_SECURITY_TEST_CRYPTOAUTH_RNG_APP_HEXDUMP_H_
#define SMING_SECURITY_TEST_CRYPTOAUTH_RNG_APP_HEXDUMP_H_

#include <SmingCore.h>


class HexDump {
public:
	HexDump();
	virtual ~HexDump();

	void print(unsigned char *data, int len);
	void resetAddr();
private:
	void pritln(char * buf, int len);
//	int width = 16;
	int addr = 0;
//	char *buf = new char(16);

};

#endif /* SMING_SECURITY_TEST_CRYPTOAUTH_RNG_APP_HEXDUMP_H_ */
