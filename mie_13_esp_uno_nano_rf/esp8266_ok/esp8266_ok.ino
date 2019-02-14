//nRF24L01 - ESP8266 12

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h> 
#include <NTPClient.h>
#include <SPI.h>
#include <RF24.h>
#include "RF24.h"     
#include <string.h> 
#include <Hash.h> 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>

#define SEALEVELPRESSURE_HPA (27.1272)
Adafruit_BMP085 bme;
float temperature, humidity, pressure, altitude; 
 
RF24 radio(2, 15);    // Set up nRF24L01 radio on SPI bus plus pins 2 for CE and 15 for CSN
   

int ti=0; //количество отправленных запросов
const char* www_username = "rafael";
const char* www_password = "23208210";
const char* ssid = "Speedy-6885BC";
const char* password = "1426110744";  


const long utcOffsetInSeconds = -10800;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "south-america.pool.ntp.org", utcOffsetInSeconds);

float h, t, s, l, m, n;
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
  
ESP8266WebServer server(80); 
void handleNotFound(){ //страница ошибок
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
 
}
void api(){         
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();   
String api = "ESP8266 + NRF2L01  \n\n";
api += "WiFi RSSI \n";
api += WiFi.RSSI();
api += "\n";
api += "\n";
api += "Hora nodo0:\n";   
api += (timeClient.getFormattedTime());
api += "\n";
api += "\n"; 
api += "Ram \n";
api += ESP.getFreeHeap();
api += "\n";
api += "\n";
api += "Chip ID\n ";
api += ESP.getChipId();
api += "\n";
api += "\n";
api += "Temperature in:\n";
api += temperature;
api += "\n";
api += "\n";
api += "Pressure in:\n";
api += pressure;
api += "\n";
api += "\n";
api += "Altitude int:\n";   // давление BMP180
api += altitude; 
api += "\n";
api += "\n";
api += "RF Sent requests:\n";
api += ti ;
api += "\n";
api += "\n";
api += "DHT\n";  
api += "Temperature ext:\n";  // Температура BMP180
api += t ; 
api += "\n";
api += "\n";
api += "Humidity ext:\n";   // давление BMP180
api += h ; 
api += "\n";
api += "\n"; 
api += "NH3 ext:\n";   // давление BMP180
api += s; 
api += "ppm\n";   // давление BMP180
api += "\n";
api += "\n"; 
api += "luminosidad ext:\n";   // давление BMP180
api += l; 
api += "lux\n";   // давление BMP180
api += "\n";
api += "\n"; 
api += "Conductivida Suelo ext:\n";   // давление BMP180
api += m; 
api += "\n";
api += "\n"; 
api += "CO ext:\n";   // давление BMP180
api += n; 
api += "\n";
api += "\n"; 
server.send(200, "text/plain", api);

 };
void setup(void){     
  SPI.setHwCs(true);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);   
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(115);                                  // Указываем канал приёма данных (от 0 до 127), 5 - значит приём данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openReadingPipe (1, 0x1234567890LL);            // Открываем 1 трубу с идентификатором 0x1234567890 для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    radio.startListening  ();                             // Включаем приемник, начинаем прослушивать открытую трубу
//  radio.stopListening   (); 
    Serial.begin(115200);  
    WiFi.mode(WIFI_STA); //режим клиент
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    bme.begin(0x77);   

  } 
    Serial.println(WiFi.localIP());   

    
server.on("/", api);
server.on("/reset", [](){                     // Перезагрузка
     if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();  
    server.send(200, "text/plain", "ok");
    ESP.restart();    
     });          
 server.onNotFound(handleNotFound);          //  Cтраница ошибки
server.on("/test", HTTP_GET, [](){          //   Обновление
       if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();  
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", serverIndex);
    });
    server.on("/update", HTTP_POST, [](){
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
      ESP.restart();
    },[](){
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();         
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
          }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress           
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
    });    
  server.begin();
  timeClient.begin();
  }
void loop(void){
 timeClient.update();

 server.handleClient();
 float data[6];   
 if(radio.available()){                                // Если в буфере имеются принятые данные
 radio.read(&data, sizeof(data));                  // Читаем данные в массив data и указываем сколько байт читать     
  h=data[0]; 
  t=data[1];
  s=data[2];  
  l=data[3];  
  m=data[4];  
  n=data[5];    
  ti++;  
 }
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
}




 
