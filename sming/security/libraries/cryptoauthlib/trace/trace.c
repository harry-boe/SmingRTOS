/*
 * trace.c
 *
 *  Created on: Jul 20, 2016
 *      Author: harry
 */

#include "trace.h"
#include <ctype.h>
#include <espressif/esp_libc.h>


char t_oc[] = "0123456789ABCDEF";
int t_addr = 0;

void _pritln(char * buf, int len) {

	uint8_t idx = 0;
	// print address
	os_printf("(%d bytes)", len);

	// Show the hex codes
	for( int i = 0; i < len; i++ )
	{
	   if ( i % 16 == 0 ) os_printf("\r\n   ");
	   if ( i % 8 == 0 ) os_printf(" ");
	   if (i < len) {
		      idx = (buf[i] & 0Xf0) >>4;
		      os_printf("%c", t_oc[idx]);
		      idx = buf[i] & 0X0f;
		      os_printf("%c ", t_oc[idx]);
	   }
	}

	os_printf("\n");
	t_addr+= len;
}


void tracef(char * label, char * buf, uint16_t len) {

	uint8_t i = 0;
	os_printf("%s :", label);

	t_addr = 0;
	_pritln(buf, len);

}

void trace(char * label) {

	uint8_t i = 0;
	os_printf("%s :\r\n", label);
}

void tr_packet(ATCAPacket packet) {

	os_printf("\nATCAPacket");
	os_printf("\n _reserved %X", packet._reserved);
	os_printf("\n opcode %X", packet.opcode);
	os_printf("\n param1 %X", packet.param1);
	os_printf("\n param2 %X", packet.param2);
	os_printf("\n txsize %d", packet.txsize);
	os_printf("\n rxsize %d", packet.rxsize);
	os_printf("\n execTime %X", packet.execTime);
	tracef("\ndata", packet.data, 130);
}

