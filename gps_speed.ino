/*
 * The sketch parses UBX messages from u-blox NEO-7M and outputs ready GPS data to a serial port in a CSV format.
 *
 * u-blox NEO-7M - Arduino Uno
 * VCC - 5V
 * RX - 3
 * TX - 2
 * GND - GND
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <UbxGpsNavPvt.h>
#include <math.h>
#include <TM1637Display.h>

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20

SoftwareSerial ss(8,9);
UbxGpsNavPvt<SoftwareSerial> gps(ss);

char datetime[DATETIME_LENGTH];

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

const uint8_t SEG_DONE[] = {
	SEG_A | SEG_F | SEG_E | SEG_D ,           // C
	SEG_A | SEG_D ,                           // =
	SEG_A | SEG_D ,                           // =
	SEG_A | SEG_B | SEG_C | SEG_D             // 反c
	};

TM1637Display display(CLK, DIO);

const unsigned char UBLOX_INIT[] PROGMEM = {
  // Disable NMEA
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24, // GxGGA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B, // GxGLL off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32, // GxGSA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39, // GxGSV off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40, // GxRMC off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47, // GxVTG off

  // Disable UBX
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0xDC, //NAV-PVT off
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x12,0xB9, //NAV-POSLLH off
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0xC0, //NAV-STATUS off

  // Enable UBX
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1, //NAV-PVT on
  //0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x13,0xBE, //NAV-POSLLH on
  //0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x14,0xC5, //NAV-STATUS on

  // Rate
  0xB5,0x62,0x06,0x08,0x06,0x00,0x64,0x00,0x01,0x00,0x01,0x00,0x7A,0x12, //(10Hz)
  //0xB5,0x62,0x06,0x08,0x06,0x00,0xC8,0x00,0x01,0x00,0x01,0x00,0xDE,0x6A, //(5Hz)
  //0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39 //(1Hz)
};



const unsigned char UBX_HEADER[] = { 0xB5, 0x62 };

void setup()
{
    Serial.begin(9600);
    ss.begin(9600);
    gps.begin(9600);
    display.setBrightness(7, true);// Turn on
    display.clear();
    display.setSegments(SEG_DONE);
    delay(1000); // 等待模块初始化
    display.clear();

    // send configuration data in UBX protocol
    for(unsigned int i = 0; i < sizeof(UBLOX_INIT); i++) {                        
      ss.write( pgm_read_byte(UBLOX_INIT+i) );
      delay(5); // simulating a 38400baud pace (or less), otherwise commands are not accepted by the device.
      }
      
      delay(1000); // 等待模块初始化

    Serial.println("start up");
    //gps.begin(115200);
}

void loop()
{
    if (gps.ready())
    {
        /*
        snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, gps.year, gps.month, gps.day, gps.hour, gps.min, gps.sec);

        Serial.print(datetime);
        Serial.print(',');
        Serial.print(gps.lon / 10000000.0, 7);
        Serial.print(',');
        Serial.print(gps.lat / 10000000.0, 7);
        Serial.print(',');
        Serial.print(gps.height / 1000.0, 3);
        Serial.print(',');
        Serial.print(gps.gSpeed * 0.0036, 5);
        Serial.print(',');
        Serial.print(gps.heading / 100000.0, 5);
        Serial.print(',');
        Serial.print(gps.fixType);
        Serial.print(',');
        Serial.println(gps.numSV);
        */
        //int num0 = gps.numSV;
        //if(num0 != 0)
        //{
        int speed = round(gps.gSpeed * 0.0036 * 10); // 四舍五入，保留一位小数 单位10km/h
        //Serial.println(speed);
        if(speed < 10)
        {
          int num = -1 * gps.numSV;
          display.showNumberDec(num, false); // Expect: ___0
          //return;
          //Serial.println("1");
        }
        //Serial.println("2");
        if( speed >= 10)
        {
      //display.showNumberDec(speed, false); // Expect: ___0
        display.showNumberDecEx(speed, (0x80 >> 2 ), false);// Expect: ___0.0
        //return;
        //Serial.println("3");
        }
        //Serial.println("4");
        //Serial.println("speed output ok");
        //Serial.println(gps.lon / 10000000.0, 7);
        //delay(50);

        //return;
      //}
    //if(num0 == 0)
    //{
    //  display.setSegments(SEG_DONE);
    //}

    }
    //if (!gps.ready())
    //{
    //Serial.println("gps error or no singal");
    //display.showNumberHexEx(0xCC);// Expect: CC// f1Af
    //}

    //snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, gps.year, gps.month, gps.day, gps.hour, gps.min, gps.sec);
    //Serial.println(datetime);
    //delay(50);
}