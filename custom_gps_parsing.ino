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

#include <HardwareSerial.h>
#include <string.h>

// what's the name of the hardware serial port?
HardwareSerial MySerial(0);
#define GPSSerial MySerial

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"

// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

#define PMTK_Q_RELEASE "$PMTK605*31"

uint32_t timer = millis();

float latitude = 0.0;
float longitude = 0.0;
float altitude = 0.0;

bool hasFix = false;

String delimeter = ",";
const char* del = delimeter.c_str();


void setup()
{
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic parsing test!");

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPSSerial.begin(9600);
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_ALLDATA); //Turn on all data (need GPGGA and GPGSA right now to make the code work)
  //If we have issues with all data, I can figure out a way to not need GPGSA
  GPSSerial.println(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Request updates on antenna status, comment out to keep quiet
  //GPSSerial.println(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  GPSSerial.println(PMTK_Q_RELEASE);
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  //THIS NEEDS TO BE FIXED TO TRY/CATCH FOR DIFFERENT INCOMING DATA
  //I think I can just read in the header and decide what to do from there
  if (GPSSerial.available()) {
    String str = GPSSerial.readStringUntil('\n');
    if (str) {
      //Serial.println(str);
      char* c = (char*) str.c_str();
      char * header = strtok(c, del);
      if (strcmp(header,"$GPGSA")==0) {
        //Use this to check if we have fix
        char * mode = strtok(nullptr, del);
        char * fix = strtok(nullptr, del);
        if (fix != "1" && !hasFix) {
          //There is a 2D (2) or 3D (3) fix
          hasFix = true;
        } else if (fix == "1" && hasFix) {
          hasFix = false;
        }
      } else if ((strcmp(header, "$GPGGA") == 0) && hasFix) {
        char * utc = strtok(nullptr, del);

        char * lat = strtok(nullptr, del); //Latitude (DDmm.mm)
        char * latDir = strtok(nullptr, del); //Latitude direction (N, S)

        char * lon = strtok(nullptr, del); //Longitude (DDDmm.mm)
        char * lonDir = strtok(nullptr, del); //Longitude direction (E, W)

        char * quality = strtok(nullptr, del); //Quality
        char * numSats = strtok(nullptr, del); //Number of Satellites in use
        char * hdop = strtok(nullptr, del); //Horizontal dilution of precision

        char * alt = strtok(nullptr, del); //Atlitude above mean sea level
        char * altUnits = strtok(nullptr, del); //Altitude units (M = meters)

        char * undulation = strtok(nullptr, del);
        char * uUnits = strtok(nullptr, del); //Undulation units
        char * ageSec = strtok(nullptr, del); //Age of correction data (seconds)

        latitude = atof(lat);
        longitude = atof(lon);
        altitude = atof(alt);
      }
    }
  }

  // approximately every 5 seconds or so, print out the current stats
  if (millis() - timer > 5000) {
    timer = millis(); // reset the timer
    Serial.print("Fix: "); Serial.println((int)hasFix);
    if (hasFix) {
      Serial.print("Location: ");
      Serial.print(latitude, 4);
      Serial.print(", ");
      Serial.println(longitude, 4);
      Serial.print("Altitude: "); Serial.println(altitude);
    }
  }
}