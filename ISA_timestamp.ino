#include <ESP32Time.h>
#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <TM1650.h>
               
TM1650 d;
int buzzer = 16;          //Assigning buzzer in IO board

/*---------Wifi credentials---------------*/
const char* ssid = "Alfa12";
const char* password = "12345678";

/*---------set with NTP---------------*/
const char* ntpServer = "pool.ntp.org";
 long  gmtOffset_sec;
 int daylightOffset_sec;


ESP32Time rtc; // initializing rtc

void setup() {
  Serial.begin(115200);
  Wire.begin();
  d.init();
  pinMode(buzzer, OUTPUT);
 
/*---------WIFI Setup---------------*/
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.println("WIFI CONNECTED");


/*---------Serial input ---------------*/
 // input for gmtoffset_sec
  Serial.println("Enter Country UTC in sec"); //gmt input
   while (Serial.available() == 0) {
  }
  long gmtOffset_sec = Serial.parseInt();

  // input for daylightOffset_sec
  Serial.println("Enter daylightOffset in sec "); //daylight input
  while (Serial.available()) Serial.read(); // flushing out old bytes from serial recieve buffer
   while (Serial.available() == 0) {
  }
  int daylightOffset_sec = Serial.parseInt();
 
 

/*---------set with NTP---------------*/
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  
  if (getLocalTime(&timeinfo)){
    rtc.setTimeStruct(timeinfo);
     
    }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

}

void loop() {
  
  struct tm timeinfo = rtc.getTimeStruct();
  Serial.println(&timeinfo);

 /*---------setting alarm at desired time---------------*/ 
   int my_hour = timeinfo.tm_hour;
   int my_min = timeinfo.tm_min;
  int my_sec = timeinfo.tm_sec;

     Serial.print(my_hour);
     Serial.print(":");
     Serial.print(my_min);
     Serial.print(":");
     Serial.println(my_sec);
     if(my_hour == 01 && my_min == 53 && my_sec == 00){
    digitalWrite(buzzer, HIGH);
      delay(3000);
      digitalWrite(buzzer, LOW);
      delay(1000);
     }

  
/*---------7 segment Display---------------*/
  char timeHM[5];
  strftime(timeHM,6, "%H%M", &timeinfo);

  d.displayOn();
    
    d.displayString(timeHM);
   d.setDot(1, true);
    d.setBrightnessGradually(TM1650_MAX_BRIGHT);
    delay(1000);
//   sprintf(char* buffer, const char* format, ...); //
    sprintf(timeHM, "%03u", &timeinfo);
}
