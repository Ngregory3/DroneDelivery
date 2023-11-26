#include <WiFi.h>
#include <WiFiClient.h>
#include <Adafruit_GPS.h>
#include <HardwareSerial.h>

HardwareSerial MySerial0(0);
#define GPSSerial MySerial0
Adafruit_GPS GPS(&MySerial0);
#define GPSECHO true

uint32_t timer = millis();
uint32_t loopcount ;

//const char* ssid = "NathanielHotSpot";
//const char* password = "sjgd0234";

const char* ssid = "GTother";
const char* password = "GeorgeP@1927";

// const char* ssid = "Michael’s iPhone (2)";
// const char* password = "12345678";

WiFiServer server(25);
boolean alreadyConnected = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); //for blinking led to show wifi status
  
  Serial.begin(115200);
  MySerial0.begin(9600, SERIAL_8N1, -1, -1);
  Serial.println("Adafruit GPS library basic parsing test!");

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  GPSSerial.println(PMTK_Q_RELEASE);

  Serial.println(WiFi.macAddress());
  
  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.println("Not Connected");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(50);
      digitalWrite(LED_BUILTIN, LOW);
      delay(50); 
  }
  Serial.println("\nConnected to the WiFi network");
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  server.begin();
  loopcount = 0;
  //GPSSerial.println(PMTK_Q_RELEASE);
}

void loop() // run over and over again
{
  WiFiClient client = server.available();
  if (client) {
    alreadyConnected = true;
    Serial.print("We have a new client");

    String incomingData = "";  // A string to store incoming data
    while (client.available()) {
      Serial.println("connected and ready to send data");
      char c = GPS.read();

      if (GPSECHO) if (c) Serial.print(c);
      if (GPS.newNMEAreceived()){
        Serial.print(GPS.lastNMEA());
        if (!GPS.parse(GPS.lastNMEA())) Serial.println("no parsing");
      }


        client.print("start:");

        client.println( (int)GPS.fix);
        client.println(GPS.latitude, 7); 
        client.println(GPS.longitude, 7);
        client.println(GPS.altitude);

      loopcount++;
      delay(500);
    }

    //client.print(incomingData);
    //incomingData = "";

  } else {

  }

}