/*
 * HexDump.h
 *
 *  Created on: Jul 8, 2016
 *      Author: harry-boe
 */

#ifndef SMING_SERVICES_HEXDUMP_HEXDUMP_H_
#define SMING_SERVICES_HEXDUMP_HEXDUMP_H_

class HexDump {
public:
	HexDump();
	virtual ~HexDump();

	void print(unsigned char *data, int len);
	void resetAddr();
private:
	void pritln(char * buf, int len);
	int addr = 0;
};

extern HexDump  dump;


#endif /* SMING_SERVICES_HEXDUMP_HEXDUMP_H_ */
