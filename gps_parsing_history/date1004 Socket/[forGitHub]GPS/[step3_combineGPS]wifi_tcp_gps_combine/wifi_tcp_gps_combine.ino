#include <SPI.h>
#include <Arduino.h>
#include <WiFi.h>
//#include <Adafruit_GPS.h>
#include <NeoSWSerial.h>
#include "NMEAGPS.h"

//Adafruit_GPS GPS(&Serial1);
NeoSWSerial gps_port( 6, 7 );
NMEAGPS gps;

const char* ssid = "GTother";
const char* password = "GeorgeP@1927";

// const char* ssid = "Chen";
// const char* password = "6789865942";

WiFiServer server(23);
boolean alreadyConnected = false;

String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void setup(){
    Serial.begin(115200);
    while (!Serial);

    int status = WL_IDLE_STATUS;
    Serial.println("\nConnecting");
    Serial.println(get_wifi_status(status));
    WiFi.begin(ssid, password);
    while(status != WL_CONNECTED){
        delay(500);
        status = WiFi.status();
        Serial.println(get_wifi_status(status));
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    // Serial.print(", MAC: ");
    // Serial.println(WiFi.macAddress());

    server.begin();

    // GPS.begin(9600);
    // Serial.print("GPS connected!");
    // // Configure the GPS module to continuously send data
    // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); 
    // delay(1000);

    gps_port.begin(9600);
}

void loop(){
  WiFiClient client = server.available();
  if (client) {
    if (!alreadyConnected) {
        // clead out the input buffer:
        client.flush();
        Serial.println("We have a new client");
        client.println("Hello, client!");
        alreadyConnected = true;
    }
    // Serial.println(client.available());
    // if (client.available() > 0) {
    //   // read the bytes incoming from the client:
    //   char thisChar = client.read();
    //   // echo the bytes back to the client:
    //   server.write(thisChar);
    //   // echo the bytes to the server as well:
    //   Serial.write(thisChar);
    // }
    // String incomingData = "";  // A string to store incoming data

    // while (client.available()) {
    //   char thisChar = client.read();
    //   incomingData += thisChar;  // Add read byte to the string
    // }

    // // Optionally, echo the data back to the client
    // client.print(incomingData);

    // Print the received data to the Serial Monitor
    // Serial.println(incomingData);
  } else {
    //Serial.println("no client");
  }


  // if (GPS.newNMEAreceived()) {
  //   Serial.println("New loop 1");
  //   if (GPS.parse(GPS.lastNMEA())) {
  //     Serial.println("New loop 2");
  //     // while (client.available()) {
  //       Serial.println("New loop");
  //       // Now you can access GPS data, for example:
  //       float latitude = GPS.latitude_fixed / 100000.0;
  //       float longitude = GPS.longitude_fixed / 100000.0;

  //       String gpsData = "Latitude: " + String(latitude, 6) + ", Longitude: " + String(longitude, 6);
  //       client.println(gpsData);  // Send the GPS data to the client
  //       delay(5000);
  //     // }
  //   }
  // }

  while (gps.available(gps_port)) {
    gps_fix fix = gps.read();
    if (fix.valid.location) {
      Serial.print(fix.latitude());
    }
  }
  delay(500);
}