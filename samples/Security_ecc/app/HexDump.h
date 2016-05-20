/*
 * HexDump.h
 *
 *  Created on: May 12, 2016
 *      Author: harry
 */

#ifndef SMING_SMING_SERVICES_HEXDUMP_HEXDUMP_H_
#define SMING_SMING_SERVICES_HEXDUMP_HEXDUMP_H_

class HexDump {
public:
	HexDump();
	virtual ~HexDump();

	void print(const char *label, unsigned char *data, int len);
	void resetAddr();
private:
	void pritln(char * buf, int len);
//	int width = 16;
	int addr = 0;
//	char *buf = new char(16);

};

#endif /* SMING_SMING_SERVICES_HEXDUMP_HEXDUMP_H_ */
