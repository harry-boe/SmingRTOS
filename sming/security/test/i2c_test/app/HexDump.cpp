/*
 * HexDump.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: harry-boe
 */

#include "HexDump.h"


HexDump::HexDump() {
	addr = 0;
}

HexDump::~HexDump() {
	// TODO Auto-generated destructor stub
}


void HexDump::print(unsigned char *data, int len) {

	Serial.printf("Data: (%d Bytes)\n", len);
	int idx = 0;
	while (len-idx > 16) {
		pritln((char *)&data[idx], 16);
		idx +=16;
	}
	pritln((char *)&data[idx], len-idx);

}


char oc[] = "0123456789ABCDEF";

void HexDump::pritln(char * buf, int len) {

	uint8_t idx = 0;
	// print address
	Serial.printf("%06X: ", addr);

	// Show the hex codes
	for( int i = 0; i < 16; i++ )
	{
	   if ( i % 8 == 0 ) Serial.printf(" ");
	   if (i < len) {
//		      Serial.printf("%02X ", buf[i]);
		      idx = (buf[i] & 0Xf0) >>4;
		      Serial.printf("%c", oc[idx]);
		      idx = buf[i] & 0X0f;
		      Serial.printf("%c ", oc[idx]);
	   }
	   else
	      Serial.printf("   ");
	}
    // Show printable characters
	Serial.printf("  |");
    for( int i = 0; i < len; i++)
    	 Serial.printf("%c", isprint(buf[i]) ? buf[i] : '.');

    Serial.printf("|\n");
	addr+= len;
}

void HexDump::resetAddr() {
	addr = 0;
}

