/* Interactive Target System (ITS)
  Target Node
  This Node will receive instructions from the master to activate a target, turn on an led and then measure the time until the
  target is hit using a piezo sensor on the steel plate. It then sends that time info back to the master to be processed.

  This device uses a NodeMCU arduino featuring the ESP8266 Wifi chip for communication between targets and the master.
 */

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "dd-wrt-workshop-morewave";	// Wifi AP to connect to
const char* password = "tributary";				// Wifi password

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char replyPacket[20] = "";  // a reply string to send back


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);	// set led pin as output


	Serial.begin(115200);
	Serial.println();



	Serial.printf("Connecting to %s ", ssid);
	WiFi.begin(ssid, password);	// Connect to Wifi AP
	while (WiFi.status() != WL_CONNECTED) // While not connected
	{
		delay(500);
		Serial.print(".");
		//TO DO: restart esp if cant connect
	}
	Serial.println(" connected");

	Udp.begin(localUdpPort);	// Begin listening on local port
	Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
	int packetSize = Udp.parsePacket();  // Returns the size of the packet in bytes, or 0 if no packets are available
	if (packetSize)	// If there is a packet
	{
		// receive incoming UDP packets
		Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
		int len = Udp.read(incomingPacket, 255);	// reads packet into our buffer and len = total bytes read
		if (len > 0)
		{
			incomingPacket[len] = 0;	// Puts Null terminator at end
		}
		Serial.printf("UDP packet contents: %s\n", incomingPacket);
		
		// TODO: PARSE incoming data into commands
		// Below is the message protocol
		// "'Set or Get' 'TargetID' 'Health or Time' 'Data'"
		// eg "S 4 H 2" for SET 4th target health to 2


		if (incomingPacket[0] == 'S') // If first byte is 'S' then command is SET
		{
			
		}
		// send back a reply, to the IP address and port we got the packet from
		Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
		Udp.write(replyPacket);
		Udp.endPacket();
	}
}