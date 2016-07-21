/*
 * trace.h
 *
 *  Created on: Jul 20, 2016
 *      Author: harry
 */

#ifndef SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_TRACE_TRACE_H_
#define SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_TRACE_TRACE_H_

#include <espressif/c_types.h>
#include "../atca_command.h"

void trace(char * label);
void tracef(char * label, char * buf, uint16_t len);
void tr_packet(ATCAPacket packet);

#endif /* SMING_SECURITY_LIBRARIES_CRYPTOAUTHLIB_TRACE_TRACE_H_ */
