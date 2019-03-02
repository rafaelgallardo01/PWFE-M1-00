#include <SoftwareSerial.h>
#define pwmPin 10
SoftwareSerial swSerial(A0, A1); // RX, TX

void setup() {
  Serial.begin(9600); 
  swSerial.begin(9600); 
  pinMode(pwmPin, INPUT);

  /*
  Источник - https://revspace.nl/MHZ19
   2000 ppm range: 0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x07, 0xD0, 0x8F
   5000 ppm range: 0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB
  */

  // Этот вариант ("A") с записью команды в 6й и 7й байт - работает
  //           bytes:                         3     4           6     7
  byte setrangeA_cmd[9] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB}; // задаёт диапазон 0 - 5000ppm
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
  delay(1000);

/*  
  // Этот вариант ("B") с записью команды в 3й и 4й байт, согласно даташиту - НЕ работает и поэтому закомментирован
  //           bytes:                         3     4           6     7
  byte setrangeB_cmd[9] = {0xFF, 0x01, 0x99, 0x00, 0x00, 0x00, 0x13, 0x88, 0xCB};
  unsigned char setrangeB_response[9]; 
  swSerial.write(setrangeB_cmd,9);
  swSerial.readBytes(setrangeB_response, 9);
  int setrangeB_i;
  byte setrangeB_crc = 0;
  for (setrangeB_i = 1; setrangeB_i < 8; setrangeB_i++) setrangeB_crc+=setrangeB_response[setrangeB_i];
  setrangeB_crc = 255 - setrangeB_crc;
  setrangeB_crc += 1;
  if ( !(setrangeB_response[0] == 0xFF && setrangeB_response[1] == 0x99 && setrangeB_response[8] == setrangeB_crc) ) {
    Serial.println("Range CRC error: " + String(setrangeB_crc) + " / "+ String(setrangeB_response[8]) + " (bytes 3 and 4)");
  } else {
    Serial.println("Range was set! (bytes 3 and 4)");
  }
  delay(1000);
*/ 

}

void loop() {

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
  delay(5000);
}
