/* 
  WiFiTelnetToSerial - Example Transparent UART to Telnet Server for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WiFi library for Arduino environment.
 
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <ESP8266WiFi.h>

//how many clients should be able to telnet to this ESP8266
#define MAX_SRV_CLIENTS 1
const char* ssid = "EnglewoodN";
const char* password = "12345678";

WiFiServer server(9100);
WiFiClient serverClients[MAX_SRV_CLIENTS];

void setup() {
  //prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  Serial.begin(19200);
  //Serial1.begin(115200);
  WiFi.softAPdisconnect(true);
  WiFi.begin(ssid, password);
  //Serial.print("\nConnecting to "); Serial.println(ssid);
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED){ 
    delay(500);
    if(WiFi.status() != WL_CONNECTED && i == 30){
      Serial.print("Need WLAN: "); Serial.println(ssid);
    }
    i++;
  }
  //start UART and the server
  server.begin();
  server.setNoDelay(true);
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println(":9100");
}

void loop() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        //Serial.print("New client: ");
        //Serial.print(i);
        //Serial.println(" connected.");
        //Serial.println("");
        continue;
      }
      delay(0);
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        digitalWrite(2, 0);
        delay(10);
        //get data from the telnet client and push it to the UART
        while(serverClients[i].available()){
          Serial.write(serverClients[i].read());
          delay(0);
        }
      }
      digitalWrite(2, 1);
    }
    delay(0);
  }
  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
     delay(0);
    }
  }
}
