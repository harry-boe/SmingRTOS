/*
 * HexDump.cpp
 *
 *  Created on: Jul 8, 2016
 *      Author: harry-boe
 */

#include "HexDump.h"
#include "espressif/c_types.h"
#include "espressif/esp_libc.h"
#include "ctype.h"


HexDump::HexDump() {
	addr = 0;
}

HexDump::~HexDump() {
	// TODO Auto-generated destructor stub
}


void HexDump::print(unsigned char *data, int len) {

	printf("\r\nData: (%d Bytes)\n", len);
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
	printf("%06X: ", addr);

	// Show the hex codes
	for( int i = 0; i < 16; i++ )
	{
	   if ( i % 8 == 0 ) printf(" ");
	   if (i < len) {
//		      Serial.printf("%02X ", buf[i]);
		      idx = (buf[i] & 0Xf0) >>4;
		      printf("%c", oc[idx]);
		      idx = buf[i] & 0X0f;
		      printf("%c ", oc[idx]);
	   }
	   else
	      printf("   ");
	}
    // Show printable characters
	printf("  |");
    for( int i = 0; i < len; i++)
    	 printf("%c", isprint(buf[i]) ? buf[i] : '.');

    printf("|\n");
	addr+= len;
}

void HexDump::resetAddr() {
	addr = 0;
}

