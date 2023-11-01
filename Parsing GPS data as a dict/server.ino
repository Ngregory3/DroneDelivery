#include <WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

HardwareSerial MySerial0(0);
#define GPSSerial MySerial0
Adafruit_GPS GPS(&MySerial0);
#define GPSECHO false

uint32_t timer = millis();
uint32_t loopcount ;

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
  loopcount = 0;
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
    //client.flush();
    alreadyConnected = true;
    Serial.print("We have a new client");
    //client.print("Hello, client!");
    delay(5000);

    String incomingData = "";  // A string to store incoming data
    while (client.available()) {
      Serial.println("connected and ready to send data");
      char c = GPS.read();

      if (GPSECHO)
        if (c) client.print(c);

        // print format: "<fix int> <GPS latitude> <GPS longitude>"
        // try to fix it so that it's once every 5 seconds 

        client.print("start:");
        //client.println(loopcount);

        //client.print("Fix: ");
        client.println( (int)GPS.fix);

        //client.print("Latitude: ");
        client.println(GPS.latitude, 7); 

        //client.print(" "); delay(200);//client.print(GPS.lat); 
        //client.print("Longitude: ");
        client.println(GPS.longitude, 7);

        //client.print("Altitude: ");
        client.println(GPS.altitude);

      loopcount++;
      delay(5000);
    }

    //client.print(incomingData);
    //incomingData = "";

  } else {

  }

}