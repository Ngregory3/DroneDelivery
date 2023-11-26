#include <WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

HardwareSerial MySerial0(0);
#define GPSSerial MySerial0
Adafruit_GPS GPS(&MySerial0);
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

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);



  Serial.println("\nConnecting");
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
    // if (!alreadyConnected) {
    //     client.flush();
    //     Serial.println("We have a new client");
    //     client.println("Hello, client!");
    //     alreadyConnected = true;
    // }
    alreadyConnected = true;
    Serial.println("We have a new client");
    client.println("Hello, client!");

    String incomingData = "";  // A string to store incoming data
    while (client.available()) {
      Serial.println("connected and ready to send data");
      char c = GPS.read();

      if (GPSECHO)
        if (c) client.print(c);

      if (GPS.newNMEAreceived()) {
        client.print(GPS.lastNMEA());
        if (!GPS.parse(GPS.lastNMEA()))
          return;
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

    client.print(incomingData);

  } else {

  }

}