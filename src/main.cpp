/*
Title: Simple Humidity and Temperature program using DHT11
Author: David Elmo Ross
Date:  Sat Feb 8th 2025
Description: This program takes a reading from the DHT11 every 2 seconds.
             The DHT11 is only accurate to the nearest % for humidity and
             the nearest degree (C) for temperature.
             ANSI escape sequences are used for screen formatting
             The humidity and temperature are also sent to the LCD.
*/

// INCLUDE SECTION
#include "arduino.h"                 // include arduino.h Library
#include <dht11.h>                   // include dht11.h library
#define DHT11PIN 4                   // DHT11 Data pin connected to Pin 4
#include "rgb_lcd.h"                 // include rgb_lcd library

#include <Wire.h>                    // include Library for Wire.h

dht11 DHT11;                         // declare DHT11 to be of class dht11
rgb_lcd lcd;
//GLOBAL VARIABLES SECTION
short read_DHT11;                    // GLOBAL read_DHT11 used to start conversion
float humid,temp,fahr;               // GLOBAL float humid, temp, and fahr
char buff[16];                       // used by sprintf for 16 char LCD display
 
void  setup()
{
  Serial.begin(9600);                // set serial baud rate to 9600 baud
  pinMode(DHT11PIN,INPUT_PULLUP);    // 10K pullup or use pinmode INPUT_PULLUP

  Wire.begin(SDA, SCL);              // attach to the IIC pin
  lcd.begin(16,2);                   // start up the LCD
  lcd.display();                     // turn on LCD display
  lcd.setCursor(0,0);                // lcd cursor to col 0 row 0 
  lcd.setRGB(0,0x80,0x80);           // set LCD screen to TEAL
  lcd.print("Humid% TmpC TmpF");     // print Title
}

void loop()
{
                                      // Screen Title (%%) gives % on screen
 printf("\e[1;1H\e[2J\e[12;23HHumid(%%) (degC)  (degF)");
 for(;;)
  {
  read_DHT11 = DHT11.read(DHT11PIN);  // reads 40 bit format
                                      // high 8 bits humid & low 8 bits humid 
                                      // high 8 bits temp & low 8 bits temp
                                      // plus 8 bit parity given by adding
                                      // all 32 bits together 8 bits at a time
                                      // typically low 8 bits humid
                                      // and low 8 low 8 bits temp are 0
                                      // since the DHT11 has accuracy only
                                      // to the nearest % for humidity and
                                      // to the nearest degree for temp
  humid=(float)DHT11.humidity;        // humid gets float of humidity value
  temp=(float)DHT11.temperature;      // temp gets float of deg C temperature
  fahr=(9.0/5.0)*(temp)+32.0;         // fahr is calculated from temp
  printf("\e[13;25H");                // Put cursor at (13,32)                                           
  printf("%4.1f\t%4.1f\t%4.1f",       // print humidity temp(C) temp(F)) to 1 dec place 
            humid,temp,fahr);
                                      // sprintf formats data into string array buff
                                      // 0xdf is the degree symbol for LCD
   sprintf(buff,"%3.1f%c %3.0f%c%c%3.0f%c%c", 
               humid,'%',temp,0xdf,
               'C',fahr,0xdf,'F');
   lcd.setCursor(0,1);                // set lcd cursor to col 1, row a
   lcd.print(buff);                   // print humid,temp,fahr on lcd
   lcd.setCursor(10,1);
   delay(2000);                       // wait 2 seconds between each reading
  }
}
