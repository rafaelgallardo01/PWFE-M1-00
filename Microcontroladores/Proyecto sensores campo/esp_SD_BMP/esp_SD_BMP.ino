
#include <SPI.h>
#include <SD.h>
#include <RF24.h>
#include "RF24.h"
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
float temperature, humidity, pressure, altitude;
RF24 radio(2, 15);

const int chipSelect = 0;
const char *ssid = "Speedy-6885BC";
const char *password = "1426110744";
int ti = 0;
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x20};
WiFiServer server(80);
float h, t, s, l, m;
void setup() {
  Serial.begin(9600);
  delay(10);
  bmp.begin(0x77); 
  delay(10);
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  // prepare GPI
  pinMode(16, OUTPUT);
  digitalWrite(16, 1);
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.println("card initialized.");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 1; analogPin++) {
    int sensor = analogRead(analogPin);
    int t = (bmp.readTemperature());
    int p = (bmp.readPressure());
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
    
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
if (client) {  // got client?
      digitalWrite(16, 0);
       Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
         // client.println("Refresh:30");  // refresh the page automatically every x sec
          client.println("meta charset=utf-8");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("<h1>Valores de lectura</h1>");
          client.print("<p><b>Los valores de A0 son: </b>");
          client.print(dataString);
          client.print("</p>");
          client.println("<br />");
          client.print("<h1>BMP_180</h1>");
          client.print("<p><b>Temeperatura= </b>");
          client.print(bmp.readTemperature());
          client.print("</p>");
          client.println("<br />");
          client.print("<p><b>Presion= </b>");          
          client.print(bmp.readPressure());
          client.print("</p>");
          client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10); 
    client.stop();
    Serial.println("client disconnected");
    digitalWrite(16, 1);
}
digitalWrite(16, 1);
delay(3000);
}
