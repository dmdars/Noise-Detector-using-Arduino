// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"#include <SPI.h>
#include <SD.h>

RTC_DS3231 rtc;


const int chipSelect = 10;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int MIC1 = A0, dB1[1000], data1;   
int MIC2 = A1, dB2[1000], data2;
int i, avg1, avg2;
void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  pinMode(MIC1, INPUT);
  pinMode(MIC2, INPUT);
  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2014, 12, 18, 10, 0, 0));
  }
   Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop () {
    
   while(i<100){
    
      dB1[i] = (analogRead(MIC1)+83)/11;
      dB2[i] = (analogRead(MIC2)+83)/11;

      data1=data1+dB1[i];
      data2=data2+dB2[i];
      
      Serial.print("Desible 1 : ");
      Serial.print(dB1[i]);
      Serial.println(" dB");
   
      Serial.print("Desible 2 : ");
      Serial.print(dB2[i]);
      Serial.println(" dB");
      delay(100);
      i++;
   }
   
      DateTime now = rtc.now();
      avg1=data1/i;
      avg2=data2/i;
      if(now.second()==0){
        
        File dataFile = SD.open("dataFile.txt", FILE_WRITE);
        Serial.println(avg1);
        Serial.println(avg2);
        dataFile.print(now.year(), DEC);
        dataFile.print('/');
        dataFile.print(now.month(), DEC);
        dataFile.print('/');
        dataFile.print(now.day(), DEC);
        dataFile.print(" ");
        dataFile.print("(");
        dataFile.print(daysOfTheWeek[now.dayOfTheWeek()]);
        dataFile.print(")");
        dataFile.print(" ");
        dataFile.print(now.hour(), DEC);
        dataFile.print(':');
        dataFile.print(now.minute(), DEC);
        dataFile.print(':');
        dataFile.print(now.second(), DEC);
        
        dataFile.print(" ");
        dataFile.print(avg1);
        
        dataFile.print(" ");
        dataFile.print(avg2);
    
        dataFile.println();
        dataFile.close();
        delay(500);
        data1=0;
        data2=0;
        i=0;
        
}
//   else {
//    Serial.println("error opening dataFile.csv");
//  }
}
