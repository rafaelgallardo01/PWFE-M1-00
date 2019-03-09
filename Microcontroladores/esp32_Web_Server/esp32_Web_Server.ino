#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
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

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 3600;


// TCP server at port 80 will respond to HTTP requests
WiFiServer server(80);
float ip;


void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup(void)
{  
    Serial.begin(115200);

    // Connect to WiFi network
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Set up mDNS responder:
    // - first argument is the domain name, in this example
    //   the fully-qualified domain name is "esp8266.local"
    // - second argument is the IP address to advertise
    //   we send our IP address on the WiFi network
    if (!MDNS.begin("esp32")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");

    // Start TCP (HTTP) server
    server.begin();
    Serial.println("TCP server started");

    // Add service to MDNS-SD
    MDNS.addService("http", "tcp", 80);
    
// Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    
}

void loop(void)
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    if (client) {  // got client?
      // send a standard http response header
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      // send web page
      client.println("<!DOCTYPE html>");
      client.println("<html>");
      client.println("<head>");
      client.println("<title>Mini Toten</title>");
      client.println("</head>");
      client.println("<body>");
      client.println("<h1>Valores</h1>");
      client.println("<p>Temperatura</p>");
//      client.println("<img src=http://eant.tech/imagenes/logo.png>");
      client.println("</body>");
      client.println("</html>");
     }
    Serial.println("");
    Serial.println("New client");
    display.drawString(3,33, "New client");
    delay(100);

    // Wait for data from client to become available
    while(client.connected() && !client.available()){
        delay(10);
    }

    // Read the first line of HTTP request
    String req = client.readStringUntil('\r');

    // First line of HTTP request looks like "GET /path HTTP/1.1"
    // Retrieve the "/path" part by finding the spaces
    int addr_start = req.indexOf(' ');
    int addr_end = req.indexOf(' ', addr_start + 1);
    if (addr_start == -1 || addr_end == -1) {
        Serial.print("Invalid request: ");
        Serial.println(req);
        return;
    }
    req = req.substring(addr_start + 1, addr_end);
    Serial.print("Request: ");
    Serial.println(req);
    client.flush();

    String s;
    if (req == "/")
    {
        IPAddress ip = WiFi.localIP();
        String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
        s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
        s += ipStr;
        s += "</html>\r\n\r\n";
        Serial.println("Sending 200");
    }
    else
    {
        s = "HTTP/1.1 404 Not Found\r\n\r\n";
        Serial.println("Sending 404");
    }

  client.print(s);
  delay(500);
  Serial.println("Done with client");
  display.drawString(3,0, "ip");
  display.drawString(3,15, req);
  display.drawString(3,46, "           ...");
  display.display();
  delay(2000);
  
  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
  delay(3000);
  printLocalTime();

   // Restart ESP
  delay(5000);
  ESP.restart();
}
