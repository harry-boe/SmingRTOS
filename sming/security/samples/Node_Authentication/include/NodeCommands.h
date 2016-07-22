/*
 * commands.h
 *
 *  Created on: Jul 22, 2016
 *      Author: harry
 */

#ifndef SMING_SECURITY_SAMPLES_NODE_AUTHENTICATION_APP_NODECOMMANDS_H_
#define SMING_SECURITY_SAMPLES_NODE_AUTHENTICATION_APP_NODECOMMANDS_H_

#include <espressif/c_types.h>



class NodeCommands
{
public:
	NodeCommands();
	virtual ~NodeCommands();

	void help(void);

	void clientProvision();
	void clientBuild();
	void hostVerifyCertChain();
	void hostGenerateChallenge();
	void clientGenerateResponse();
	void hostVerifyResponse();

	// utilty functions
	void info(void);
	void sernum(void);
	void random(void);
	void lockstat(void);
	void lockcfg(void);
	void lockdata(void);

private:
	void printbuf(uint8_t * buf, int len);

	char _oc[16] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

};



#endif /* SMING_SECURITY_SAMPLES_NODE_AUTHENTICATION_APP_NODECOMMANDS_H_ */
