#include <LiquidCrystal.h>
#include <Wire.h>   // Needed for LCD include.
#include <DS3231.h>
#include <dht.h>
#include <SPI.h>  //  Needed for SD library.
#include <SD.h>

LiquidCrystal lcd(2, 3, 5, 4, 6, 7);  //  Pins LCD is connected to.
DS3231 clock;   //  Start clock object.
RTCDateTime dt;   //  Start date object.
dht DHT;  // Start sensor object.
File myLog;   // Start file object.

const String _file = ("log.txt");   //  File everything is logged to. Change name if any errors occur in output.
int temp; //  Stores temperature data.
int humi; //  Stores humidity data.

void setup()  {   //  Set up.
  Serial.begin(9600); //  Start serial comm.
  SD.begin(10);   //  Start SD comm.
  lcd.begin(16, 2);   //  Start LCD.
  clock.begin();    //  Start RTC.
  //__DATE__, __TIME__);  //  Uncomment only if date needs to be reset.
  //  Functions
  logFile();  //  Output to serial.
  logFormat();  //  File formatting.
}
void logFile() {    //  Outputs logfile to serial.
  myLog = SD.open(_file);   //  Open the file.
  if (myLog) {
    while (myLog.available()) {   //  Writes file to serial.
      Serial.write(myLog.read());
    }
    myLog.close();
    SD.remove(_file);   //  Deletes file.
  }
}
void logFormat()  {   // Formats new log file.
  myLog = SD.open(_file, FILE_WRITE);
  myLog.println("D,Ti,Te,H"); // Date, Time, Temperature, Humidity format for .csv file.
  myLog.close();
}
void loop() {   //  Loop function.
  DHT.read11(A0);   //  Reads temperature and humidty data and returns.
  temp = DHT.temperature;   // Stores temperature value as an int into temp variable.
  humi = DHT.humidity;      // Same but for humidity.
  dt = clock.getDateTime();   //  Reads RTC data and returns to dt variable.
      //  Functions
  lcdRw();    //  Prints sensor data to LCD display
  logCard();    //  Writes sensor data to SD card log file.
  delay(300000);    //  Update interval. 60000 = 1 minute
}
void  lcdRw()  {    // LCD print out
  lcd.print("READING AT ");
  lcd.print(clock.dateFormat("H:i", dt));
  lcd.setCursor(0, 1);    // Sets LCD cursor. Column, Row (1st line (0, 0) 2nd line (0,1)).
  lcd.print("  T:");
  lcd.print(temp);
  lcd.print("C");
  lcd.print("  H:");
  lcd.print(humi);
  lcd.print("%");
  lcd.setCursor(0, 0);
}
void logCard()  {   // Writes readings to SD Card.
  myLog = SD.open(_file, FILE_WRITE);   //  Opens log file with write permission.
  myLog.print(clock.dateFormat("d.m.y", dt));   //  Day Month Year
  myLog.print(",");
  myLog.print(clock.dateFormat("H:i", dt));   //  Hour Minute
  myLog.print(",");
  myLog.print(temp);
  myLog.print(",");
  myLog.println(humi);
  myLog.close();
}
