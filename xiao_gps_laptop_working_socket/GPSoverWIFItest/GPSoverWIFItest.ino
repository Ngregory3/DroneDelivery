// Test code for Ultimate GPS Using Hardware Serial (e.g. GPS Flora or FeatherWing)
//
// This code shows how to listen to the GPS module via polling. Best used with
// Feathers or Flora where you have hardware Serial and no interrupt
//
// Tested and works great with the Adafruit GPS FeatherWing
// ------> https://www.adafruit.com/products/3133
// or Flora GPS
// ------> https://www.adafruit.com/products/1059
// but also works with the shield, breakout
// ------> https://www.adafruit.com/products/1272
// ------> https://www.adafruit.com/products/746
//
// Pick one up today at the Adafruit electronics shop
// and help support open source hardware & software! -ada
#include <WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

HardwareSerial MySerial0(0);
// what's the name of the hardware serial port?
#define GPSSerial MySerial0

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&MySerial0);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

uint32_t timer = millis();


const char* ssid = "GTother";
const char* password = "GeorgeP@1927";

WiFiServer server(25);
boolean alreadyConnected = false;

void setup()
{
  Serial.begin(115200);
  MySerial0.begin(9600, SERIAL_8N1, -1, -1);
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);

/***********************************
************************************
CONNECTING TO GT_OTHER
************************************
************************************/

    Serial.println("\nConnecting");
    //delay(10000);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.println("Not Connected");
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() // run over and over again
{
  WiFiClient client = server.available();
  if (client) {
    if (!alreadyConnected) {
        // clear out the input buffer:
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
    String incomingData = "";  // A string to store incoming data

    while (client.available()) {
      Serial.println("connected and ready to send data");
      //char thisChar = client.read();
      //incomingData += thisChar;  // Add read byte to the string

            // read data from the GPS in the 'main loop'
      char c = GPS.read();
      // if you want to debug, this is a good time to do it!
      if (GPSECHO)
        if (c) client.print(c);
      // if a sentence is received, we can check the checksum, parse it...
      if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trying to print out data
        client.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
        if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
          return; // we can fail to parse a sentence in which case we should just wait for another
      }

      // approximately every 2 seconds or so, print out the current stats
        client.print("\nTime: ");
        if (GPS.hour < 10) { client.print('0'); }
        client.print(GPS.hour, DEC); client.print(':');
        if (GPS.minute < 10) { client.print('0'); }
        client.print(GPS.minute, DEC); client.print(':');
        if (GPS.seconds < 10) { client.print('0'); }
        client.print(GPS.seconds, DEC); client.print('.');
        if (GPS.milliseconds < 10) {
          client.print("00");
        } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
          client.print("0");
        }
        
        client.println(GPS.milliseconds);

        client.print("<br>");
        client.print("Date: ");
        client.print(GPS.day, DEC); client.print('/');
        client.print(GPS.month, DEC); client.print("/20");
        client.println(GPS.year, DEC); client.print("<br>");
        client.print("Fix: "); client.print((int)GPS.fix); client.print("<br>");
        client.print(" quality: "); client.println((int)GPS.fixquality); client.print("<br>");

        client.print("<br>");
        if (GPS.fix) {
          client.print("Location: ");
          client.print(GPS.latitude, 4); client.print(GPS.lat);
          client.print(", ");
          client.print(GPS.longitude, 4); client.println(GPS.lon);client.print("<br>");
          client.print("Speed (knots): "); client.println(GPS.speed);client.print("<br>");
          client.print("Angle: "); client.println(GPS.angle);client.print("<br>");
          client.print("Altitude: "); client.println(GPS.altitude);client.print("<br>");
          client.print("Satellites: "); client.println((int)GPS.satellites);client.print("<br>");
          client.print("Antenna status: "); client.println((int)GPS.antenna);client.print("<br>");
        }
        client.print("<br>");
      delay(1000);
    }

    // // Optionally, echo the data back to the client
    client.print(incomingData);

    // Print the received data to the Serial Monitor
    //Serial.println(incomingData);
  } else {
    //Serial.println("no client");
  }

}