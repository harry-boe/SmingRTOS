#include <user_config.h>
#include <SmingCore.h>
#include <libraries/PackedRadio/RH_RF95.h>
#include <libraries/PackedRadio/RHHardwareSPI.h>
#include <libraries/PackedRadio/RHGenericSPI.h>
#include <libraries/PackedRadio/RHReliableDatagram.h>


/*(!) Warning on some hardware versions (ESP07, maybe ESP12)
 * 		pins GPIO4 and GPIO5 are swapped !*/
#define PIN_RF95_DO  12	/* Master In Slave Out */
#define PIN_RF95_DI  13	/* Master Out Slave In */
#define PIN_RF95_CK  14	/* Serial Clock */
#define PIN_RF95_SS  0	/* Slave Select */
#define PIN_RF95_INT 4	/* Interrupt pin */

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// If you want, you can define WiFi settings globally in Eclipse Environment Variables
#ifndef WIFI_SSID
	#define WIFI_SSID "PleaseEnterSSID" // Put you SSID and Password here
	#define WIFI_PWD "PleaseEnterPass"
#endif

// ... and/or MQTT username and password
#ifndef MQTT_USERNAME
	#define MQTT_USERNAME ""  			// for pubnub this is the publish_key
	#define MQTT_PWD ""					// for pubnub this is the subscribe_key
#endif

#define MQTT_UUID "esp8266-LORA"	// for pubnub this is a unique client identifier

// ... and/or MQTT host and port
#ifndef MQTT_HOST
	#define MQTT_HOST "mqtt.pubnub.com"
	#define MQTT_PORT 1883
#endif

// Forward declarations
void startMqttClient();
void onMessageReceived(String topic, String message);

String user = MQTT_USERNAME;
String pwd  = MQTT_PWD;
String channel = "Channel-f4dqakl46";



Timer procTimer, msgTimer;


RHHardwareSPI spi = RHHardwareSPI();

// Singleton instance of the radio driver
//RH_RF95 driver;
RH_RF95 driver(PIN_RF95_SS, PIN_RF95_INT, spi);

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, SERVER_ADDRESS);

// MQTT client
// For quick check you can use: http://www.hivemq.com/demos/websocket-client/ (Connection= test.mosquitto.org:8080)
MqttClient mqtt(MQTT_HOST, MQTT_PORT, onMessageReceived);


// Dont put this on the stack:
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

//uint8_t data[] = "And hello back to you";

#define LED_PIN 2 // GPIO2
int led = LED_PIN;

void setupRF95()
{

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  if (!driver.init())
    Serial.println("driver init failed");
  if (!manager.init())
    Serial.println("manager init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  driver.setTxPower(23, false);
}

// Check for MQTT Disconnection
void checkMQTTDisconnect(TcpClient& client, bool flag){

	// Called whenever MQTT connection is failed.
	if (flag == true)
		Serial.println("MQTT Broker Disconnected!!");
	else
		Serial.println("MQTT Broker Unreachable!!");

	// Restart connection attempt after few seconds
	procTimer.initializeMs(2 * 1000, startMqttClient).start(); // every 2 seconds
}


// Run MQTT client
void startMqttClient()
{

	procTimer.stop();
	if(!mqtt.setWill("last/will","The connection from this device is lost:(", 1, true)) {
		debugf("Unable to set the last will and testament. Most probably there is not enough memory on the device.");
	}
	// in this case wr connect to pubnub - should work with any other mqtt server
	// mqtt_connect = "<publish_key>/<subscribe_key>/<client_uuid>"
	mqtt.connect(MQTT_UUID, MQTT_USERNAME, MQTT_PWD);

	// Assign a disconnect callback function
	mqtt.setCompleteDelegate(checkMQTTDisconnect);
	String topic = user + '/' + pwd + '/' + channel;
	mqtt.subscribe(topic);

//	mqtt.commandProcessing(true,"command","cmdreply");
}

// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
	Serial.print(topic);
	Serial.print(":\r\n\t"); // Pretify alignment for printing
	Serial.println(message);
}


// Publish our message
void publishMessage()
{
	if (mqtt.getConnectionState() != eTCS_Connected)
		startMqttClient(); // Auto reconnect

	String topic = user + '/' + pwd + '/' + channel;

	Serial.println("Let's publish message now!");
	mqtt.publish(topic, "Hello friends, from Internet of things :)"); // or publishWithQoS
}





void reliableMsgRF95() {
	  Serial.println("reliableMsgRF95");
	  if (manager.available())
	  {
	    // Wait for a message addressed to us from the client
	    uint8_t len = sizeof(buf);
	    uint8_t from;
	    // show the LORA activity on LED
	    digitalWrite(led, LOW);
	    if (manager.recvfromAck(buf, &len, &from))
	    {
	  	  digitalWrite(led, HIGH);
	      Serial.print("got request from : 0x");
	      Serial.print(from, HEX);
	      Serial.print(": ");
	      Serial.println((char*)buf);
	      Serial.print("RSSI: ");
	      Serial.println(driver.lastRssi(), DEC);

	      // Send a reply back to the originator client
	      uint8_t data[] = "hello LORA";
		    // show the LORA activity on LED
		  digitalWrite(led, LOW);
	      if (!manager.sendtoWait(data, sizeof(data), from))
	        Serial.println("sendtoWait failed");
	  	  digitalWrite(led, HIGH);
	    }
	  }
}


// Will be called when WiFi station was connected to AP
void connectOk()
{
	Serial.println("I'm CONNECTED");

	// init rf95 module
	setupRF95();

	// Run MQTT client
	startMqttClient();

	// Start publishing loop
	procTimer.initializeMs(20 * 1000, publishMessage).start(); // every 20 seconds

	// Start the LORA timer
	msgTimer.initializeMs(2 * 1000, reliableMsgRF95).start(); // every 20 seconds
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
