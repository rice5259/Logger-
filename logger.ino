#include <dht.h>
#include <SPI.h>
#include <SD.h>
dht DHT;
File myLog;

//Set up
void setup()  {
  Serial.begin(9600);
  Serial.println("Temperature Logger v1.0");
  Serial.println();
  delay(1500);
  sdInit();
  logOut();
  logFormat();
}

//SD card initialization.
void sdInit() {
  if (!SD.begin(10))  {
    Serial.println("SD Reader failed to initiate: Program terminated...");
    while (1);
  }  else {
    Serial.println("SD Card detected...");
    delay(1500);
  }
}

//Outputs logfile to serial.
void logOut() {
  if (SD.exists("log.txt")) {
    myLog = SD.open("log.txt");
    Serial.println("CONTENTS OF LOG FILE:");
    delay(3000);
    while (myLog.available()) {
      Serial.write(myLog.read());
    }
    delay(1500);
    Serial.println("END OF LOG FILE...");
    delay(1500);
    myLog.close();
  } else  {
    Serial.println("Error during file deletion");
    //while (1);
  }
}

//Formats new log file.
void logFormat()  {
  Serial.println("Preparing to format log file...");
  delay(5000);
  SD.remove("log.txt");
  myLog = SD.open("log.txt", FILE_WRITE);
  myLog.println("Time,Temperature,Humidity");
  myLog.close();
}

//Writes readings to SD Card.
void logCard() {
  myLog = SD.open("log.txt", FILE_WRITE);
  myLog.print(DHT.temperature);
  myLog.print(",");
  myLog.print(DHT.humidity);
  myLog.println(",");
  myLog.close();
}

//Prints readings to serial.
void logSerial()  {
  Serial.print("Temperature is ");
  Serial.print(DHT.temperature);
  Serial.print(" and ");
  Serial.print("Humidity is ");
  Serial.println(DHT.humidity);
}

// Time between readings in ms.
void interval() {
  delay(60000);
}

//Loop function
void loop() {

  DHT.read11(A0);
  logCard();
  logSerial();
  interval();
}
