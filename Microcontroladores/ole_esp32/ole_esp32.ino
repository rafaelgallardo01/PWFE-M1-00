uint64_t chipid; 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "SSD1306.h" 
#include <WiFi.h>
#include "time.h" 
SSD1306  display(0x3c, 21, 22);


#define OLED_RESET 4

const char *www_username = "rafael";
const char *www_password = "23208210";
const char *ssid = "Speedy-6885BC";
const char *password = "1426110744";
WiFiServer server(80);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 3600;



void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup(){
  Serial.begin(115200);
    // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
    // Print the IP address
  Serial.println(WiFi.localIP());
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);

}

void loop(){
  display.drawString(3,0, "ESP32");
  display.drawString(3,15, ssid);
  display.drawString(3,33, "IP");
  display.drawString(3,46, "           ...");
  display.display();
  delay(2000);
  
  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  delay(3000);
  printLocalTime();

   // Restart ESP
  delay(5184000);
  digitalWrite(2, 0);
  ESP.restart();
  
  }
