#include "Wire.h" // For I2C
#include "LCD.h" // For LCD
#include "LiquidCrystal_I2C.h" // Added library*
//Set the pins on the I2C chip used for LCD connections
//ADDR,EN,R/W,RS,D4,D5,D6,D7
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#define pwmPin 10
SoftwareSerial swSerial(A0, A1); // RX, TX

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the default I2C bus address of the backpack-see article

#define SEALEVELPRESSURE_HPA (27.1272)
Adafruit_BMP085 bmp;

float temperature, humidity, pressure, altitude, Co2;
float h, t, s, l, m, th;

void setup()
{

   Serial.begin(9600);   
   
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  delay(7000);
  pinMode(pwmPin, INPUT);
  digitalWrite(pwmPin, HIGH);
  byte setrangeA_cmd[9] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
  unsigned char setrangeA_response[9]; 
  swSerial.write(setrangeA_cmd,9);
  swSerial.readBytes(setrangeA_response, 9);
  int setrangeA_i;
  byte setrangeA_crc = 0;
  for (setrangeA_i = 1; setrangeA_i < 8; setrangeA_i++) setrangeA_crc+=setrangeA_response[setrangeA_i];
  setrangeA_crc = 255 - setrangeA_crc;
  setrangeA_crc += 1;
  if ( !(setrangeA_response[0] == 0xFF && setrangeA_response[1] == 0x99 && setrangeA_response[8] == setrangeA_crc) ) {
    Serial.println("Range CRC error: " + String(setrangeA_crc) + " / "+ String(setrangeA_response[8]) + " (bytes 6 and 7)");
  } else {
    Serial.println("Range was set! (bytes 6 and 7)");
  }
  delay(100);  
   // Set off LCD module
   lcd.begin (20,4); // 16 x 2 LCD module
   lcd.setBacklightPin(3,POSITIVE); // BL, BL_POL
   lcd.setBacklight(HIGH);
   lcd.print("Mini Totem");
   delay(3000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Mini Totem...");
   delay(1000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Temperatura");   
   lcd.setCursor(0,1);
   lcd.print("Humedad");   
   lcd.setCursor(0,2);
   lcd.print("NH1");  
   lcd.setCursor(0,3);
   lcd.print("Co2");
   delay(1000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Temperatura..");   
   lcd.setCursor(0,1);
   lcd.print("Humedad..");   
   lcd.setCursor(0,2);
   lcd.print("NH1..");  
   lcd.setCursor(0,3);
   lcd.print("Co2..");
   delay(500);   
  
}
void loop(){
  

  float s = analogRead(A2);
  float l= analogRead(A2);

  temperature = bmp.readTemperature();
  pressure = bmp.readPressure() / 100.0F;
  altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);

  
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");

  byte measure_cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
  unsigned char measure_response[9]; 
  unsigned long th, tl, ppm = 0, ppm2 = 0, ppm3 = 0;

  // ***** узнаём концентрацию CO2 через UART: ***** 
  swSerial.write(measure_cmd,9);
  swSerial.readBytes(measure_response, 9);
  int i;
  byte crc = 0;
  for (i = 1; i < 8; i++) crc+=measure_response[i];
  crc = 255 - crc;
  crc += 1;
  if ( !(measure_response[0] == 0xFF && measure_response[1] == 0x86 && measure_response[8] == crc) ) {
    Serial.println("CRC error: " + String(crc) + " / "+ String(measure_response[8]));
  } 
  unsigned int responseHigh = (unsigned int) measure_response[2];
  unsigned int responseLow = (unsigned int) measure_response[3];
  ppm = (256*responseHigh) + responseLow;

 
  // *****  узнаём концентрацию CO2 через PWM: ***** 
  do {
    th = pulseIn(pwmPin, HIGH, 1004000) / 1000;
    tl = 1004 - th;
    ppm2 =  2000 * (th-2)/(th+tl-4); // расчёт для диапазона от 0 до 2000ppm 
    ppm3 =  5000 * (th-2)/(th+tl-4); // расчёт для диапазона от 0 до 5000ppm 
  } while (th == 0);

  Serial.print(ppm);
  Serial.print(" <- ppm (UART) ");
  Serial.print((ppm/5)*2);
  Serial.println(" <- two fifths of it"); // Потом пришло озарение
  Serial.print(th);
  Serial.println(" <- Milliseconds PWM is HIGH");
  Serial.print(ppm2);
  Serial.println(" <- ppm2 (PWM) with 2000ppm as limit");
  Serial.print(ppm3);
  Serial.println(" <- ppm3 (PWM) with 5000ppm as limit");

  Serial.println("-----------");
   delay(100);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("T=");   
   lcd.print(bmp.readTemperature());
   lcd.setCursor(10,0);
   lcd.print("P=");
   lcd.print(bmp.readSealevelPressure());    
   lcd.setCursor(0,1);
   lcd.print("Cs="); 
   lcd.print(s);  
   lcd.setCursor(10,1);
   lcd.print(" Co2=");
   lcd.print(th);
   lcd.setCursor(0,2);
   lcd.print("Alt=");
   lcd.print(bmp.readAltitude(101500));
   lcd.setCursor(10,2);
   lcd.print("HX=");
   lcd.print(h*t/600); 
   // Escribimos el número de segundos trascurridos
   lcd.setCursor(0,3);
   lcd.print(millis()/1000);
   lcd.print("/s");



    
    Serial.println();
    delay(3000);
//    lcd.noBacklight();

   
  }
