#include <user_config.h>
#include <SmingCore.h>
#include <libraries/PackedRadio/RH_RF95.h>
#include <libraries/PackedRadio/RHReliableDatagram.h>


/*(!) Warning on some hardware versions (ESP07, maybe ESP12)
 * 		pins GPIO4 and GPIO5 are swapped !*/
#define PIN_RF95_DO 12	/* Master In Slave Out */
#define PIN_RF95_DI 13	/* Master Out Slave In */
#define PIN_RF95_CK 14	/* Serial Clock */
#define PIN_RF95_SS 4	/* Slave Select */

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2


// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

Timer procTimer;

// Singleton instance of the radio driver
RH_RF95 driver;

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

uint8_t data[] = "And hello back to you";

#define LED_PIN 2 // GPIO2
int led = LED_PIN;

void setupRF95()
{

  pinMode(led, OUTPUT);
  if (!driver.init())
    Serial.println("driver init failed");
  if (!manager.init())
    Serial.println("manager init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
//  driver.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true.
  // Failure to do that will result in extremely low transmit powers.
//  driver.setTxPower(14, true);
}

void echoRF95()
{
  if (driver.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (driver.recv(buf, &len))
    {
      digitalWrite(led, HIGH);
//      RH_RF95::printBuffer("request: ", buf, len);
      Serial.print("got request: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
//      uint8_t data[] = "And hello back to you";
      driver.send(data, sizeof(data));
      driver	.waitPacketSent();
      Serial.println("Sent a reply");
       digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}

void reliableMsgRF95() {
	  if (manager.available())
	  {
	    // Wait for a message addressed to us from the client
	    uint8_t len = sizeof(buf);
	    uint8_t from;
	    if (manager.recvfromAck(buf, &len, &from))
	    {
	      Serial.print("got request from : 0x");
	      Serial.print(from, HEX);
	      Serial.print(": ");
	      Serial.println((char*)buf);

	      // Send a reply back to the originator client
	      if (!manager.sendtoWait(data, sizeof(data), from))
	        Serial.println("sendtoWait failed");
	    }
	  }
}


// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");

	// init rf95 module
	setupRF95();

	// Start echo
	procTimer.initializeMs(20 * 1000, echoRF95).start(); // every 20 seconds
	// Start echo
	procTimer.initializeMs(30 * 1000, reliableMsgRF95).start(); // every 20 seconds
}


// Will be called when WiFi station timeout was reached
void connectFail()
{
	Serial.println("I'm NOT CONNECTED. Need help :(");

	// .. some you code for device configuration ..
}

void init()
{
	Serial.begin(SERIAL_BAUD_RATE); // 115200 by default
	Serial.systemDebugOutput(true); // Debug output to serial
	commandHandler.registerSystemCommands();

	WifiStation.config(WIFI_SSID, WIFI_PWD);
	WifiStation.enable(true);
	WifiAccessPoint.enable(false);

	// Run our method when station was connected to AP (or not connected)
	WifiStation.waitConnection(connectOk, 20, connectFail); // We recommend 20+ seconds for connection timeout at start
}
