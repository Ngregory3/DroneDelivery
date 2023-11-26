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
#include <WiFiAP.h>
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

const char *ssid = "XIAO_ESP32S3";
const char *password = "password";
WiFiServer server(80);


void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
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

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() // run over and over again
{

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    //client.println("HTTP/1.1 200 OK");
    //client.println("Content-type:text/html");
    //client.println();

    while (client.connected()) {            // loop while the client's connected

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
       /* client.print("\nTime: ");
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

       
        client.print("Date: ");
        client.print(GPS.day, DEC); client.print('/');
        client.print(GPS.month, DEC); client.print("/20");
        client.println(GPS.year, DEC); 
        client.print("Fix: "); client.print((int)GPS.fix);
        client.print(" quality: "); client.println((int)GPS.fixquality);*/


        // print format: "<fix int> <GPS latitude> <GPS longitude>"
        // try to fix it so that it's once every 5 seconds 
        client.print("Fix: "); client.print((int)GPS.fix);
        if (GPS.fix) {
          client.print(" ");
          client.print(GPS.latitude, 4); client.print(GPS.lat);
          client.print(" ");
          client.print(GPS.longitude, 4); client.println(GPS.lon);
          /*client.print("Speed (knots): "); client.println(GPS.speed);
          client.print("Angle: "); client.println(GPS.angle);
          client.print("Altitude: "); client.println(GPS.altitude);
          client.print("Satellites: "); client.println((int)GPS.satellites);
          client.print("Antenna status: "); client.println((int)GPS.antenna);*/
        }
      delay(1000);
    }
    Serial.println("Client Disconnected");
  }

}
