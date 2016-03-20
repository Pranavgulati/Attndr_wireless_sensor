/*
Name:		Attndr-wireless sensor.ino
Created:	19-Mar-16 10:46:00 AM
Author:	Pranav
*/
#define USE_SERIAL Serial
#define DEBUG
#define DEBUG_HTTPCLIENT Serial.printf

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>


#define URL "www.mysh.net23.net"
#define PORT 80

#define MY_PWD "passphrase"
#define MY_PREFIX "Sensor"
#define SERVER_IP_ADDR "192.168.4.1"
#define SERVER_PORT 2123
String ssid, password;

byte saved = 0;
#define SSIDlen 32
#define PASSWORDlen 20
ESP8266WebServer server(SERVER_PORT);// to initialise a server that connects at port 80 HTTP
String sendGET(const char url[], int port, const char path[], int length){
	String recv;
	String temp;
	temp.reserve(120);
	WiFiClient client;
	delay(100);
	if (client.connect(url, port)){
		client.print("GET ");
		client.print(path);
		client.print(" HTTP/1.1\r\n");
		client.print("Host: ");
		client.print(url);
		client.print("\r\nUser-Agent: Mozilla/5.0\r\n");
		client.print("User-Agent: Mozilla/5.0\r\n");
		client.print("Accept: text/html\r\n\r\n");
		Serial.println("WOW connected");
	
		Serial.print("GET ");
		Serial.print(path);
		Serial.print(" HTTP/1.1\r\n");
		Serial.print("Host: ");
		Serial.print(url);
		Serial.print("\r\nUser-Agent: Mozilla/5.0\r\n");
		Serial.print("User-Agent: Mozilla/5.0\r\n");
		Serial.print("Accept: text/html\r\n\r\n");
		Serial.println("WOW connected");
		//GET / hello.htm HTTP / 1.1
		//	User - Agent: Mozilla / 4.0 (compatible; MSIE5.01; Windows NT)
		//Host : www.tutorialspoint.com
		//	   Accept - Language : en - us
		//	   Accept - Encoding : gzip, deflate
		//   Connection : Keep - Alive
	}
	while (client.connected() && client.available()){

		temp += (char)client.read();
		Serial.print(".");
	}
	int wait = 1000;
	while (client.connected() && !client.available() && wait--){
		delay(3);
		
	}
	while (client.connected() && client.available()){
		
		temp += (char)client.read();
		Serial.print(".");
	}
	
	if (temp.indexOf("@") != -1){
		recv = temp.substring(temp.indexOf("@")+1, temp.lastIndexOf("@"));
	}

	Serial.println(temp);
	return recv;
}
void sendResponse(const char* response, int length){
	server.send(200, "text/html", response);
}

void configuration()
{
		ssid = server.arg("ssid");
		password = server.arg("pwd");
		WiFi.begin(ssid.c_str(), password.c_str());
		delay(1000);
		if (WiFi.waitForConnectResult() == WL_CONNECTED ){
			server.send(200, "text/html", "ACK config.Connect wifi Success");
			byte i = 0;
			}
		else{ server.send(200, "text/html", "ACK config.not connect"); }
		
		USE_SERIAL.print(ssid);
		USE_SERIAL.print("<-ssid  pwd->");
		USE_SERIAL.print(password);
		
	}


void setup() {
	
	ssid.reserve(SSIDlen);
	password.reserve(PASSWORDlen);
	ssid = "NETGEAR";
	password = "passphrase";
	USE_SERIAL.begin(115200);
	
	byte i = 0;
	USE_SERIAL.setDebugOutput(true);
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(((String)MY_PREFIX).c_str(), ((String)MY_PWD).c_str());
	Serial.println(ssid.c_str());
	Serial.println(password.c_str());
	WiFi.begin(ssid.c_str(), password.c_str());
	server.on("/config", HTTP_GET, configuration);
	server.begin();
	for (uint8_t t = 5; t > 0; t--) {
		USE_SERIAL.printf("SETUP connecting to router %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}
	if (WiFi.waitForConnectResult() == WL_CONNECTED){
		Serial.println(WiFi.localIP());
	}
}

long timers = 0;
void loop() {
	server.handleClient();
	if (Serial.available() > 3){ Serial.flush(); }
	if (WiFi.status() != WL_CONNECTED){
		WiFi.begin(ssid.c_str(), password.c_str()); //loaded from the eeprom 
		delay(1000);
		}
	else if (WiFi.status() == WL_CONNECTED && USE_SERIAL.available()>0&& USE_SERIAL.available()<3)  {
		delay(500);
			String path = "/sensor.php?number=";
			String input;
			input.reserve(3);
			while (Serial.available() > 0){
				input += Serial.read()-48;
			}
			path += input;
			String recv;
			Serial.println(path);
				recv = sendGET(URL, PORT, path.c_str(), path.length());
				Serial.println(recv);
				if (recv.length() < 4){ Serial.println("failed"); };
			
			
		}
	
	//Serial.println(timers % 5);//send random data just to ensure that the module is healthy
	delay(1000);
}




