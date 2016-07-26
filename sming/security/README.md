# support for the Atmel CryptoAuthentication devices and Libraries

Based on: Sming_RTOS Beta version
Based on: Espressif RTOS_SDK 1.4.0
Based on: Atmel CryptoAuthLib Firmware Library 20160108


Currently tested with ATECC508A device but should work with other devices as well

The library port is as less intrusiv as possible.

The main modifications are:

-	Declare the required includes whee needed. 
	All includes refer files from the espressif SDK. Sming specif includes are only used when absolutly neccesary.
	
-	To get thing compiled it required one modification on the datastructures.
	More specific the naming the union on atca_iface.h (line 106)
	And obviously all references to it.

-	Added a esp8266 specific HAL imlpementation for I2C including specialzed Timers
	Other protocolls such as SPI, UART ore onewire are not yet implemented
	
-	The CryptoAuthLib is written in C so C++ classes witout specific extern "C" declrations cannot be used.
	This is the reason why the HAL imlementation is based on the two-wire I2C core imlementation in
	twi.h / core_esp8266_si2c.cpp as this is the only place within sming where i found the compatible "C" 
	function calls.
	
- 	finaly the port of the Atmel Node_Authetication sample has lot's of changes to improove memory hadling
	for RTOS. The Atmel code uses lots of huge (up to 1024 Bytes) display buffers to presset the test results.
	This is obviously forcing RTOS heap overflows. 
	Display output has bin replace with a HexDump utility that can nouw be found in the sming/services folder
	
Building the samples

-	All security related code nucluding the samples are located in it's own folder within the sming folder
-	To build and use the security modules simply type make in the security folder
	This will create a library that will be linked against the application code
-	The samples work like an other samples just out of the folders where they are located

Note 

- 	building against rBoot is not yet support not tested. But it should work by simply modifiing the rBoot 
	Makefile (including the security lib)
	
Required Hardware

-	Obviously the solution erquires an I2C ATECC508A cryptochip to work with
	The can be sourced from Atmel, Mouser or other source for less than 1$
	I recommend bying enough of those chips. Becaouse once the got personalized (provisioned with key
	material) they get locked and set to read only mode.
-	Brakout PCB
	I'm using "SMT BREAKOUT PCB FOR SOIC-8" from Adafrout to get the chips comaptible with breakout boards.
	Soldering them is easier as I initialy thought.
-	Other more expensife options are the "AT88CK101 Development Kit" from atmel that have a socket to hold the
	chip and connectors to wire them with the ESP 
	
	

	
	
	
	