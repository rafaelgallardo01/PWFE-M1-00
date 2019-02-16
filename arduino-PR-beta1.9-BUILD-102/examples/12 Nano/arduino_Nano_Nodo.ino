/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
        == Node 02 (Child of Master node 00) ==    
*/
#include <RF24.h>
#include <SPI.h>
RF24 radio(9, 10);               // nRF24L01 (CE,CSN)


void setup() {
  SPI.begin();
  radio.begin();
  radio.setChannel(115);   
  radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe (0x1234567890LL);        
}
void loop(){
  float data[6];   
  delay(2000);  
  float h = analogRead(6);
  float t = analogRead(6);
  float s = analogRead(6);
  float l = analogRead(6);
  float m = analogRead(A0);
  float n = analogRead(A1);
    
  data[0] = h, DEC; 
  data[1] = t, DEC;
  data[2] = s, DEC; 
  data[3] = l, DEC;
  data[4] = m, DEC; 
  data[5] = n, DEC;
  radio.write(&data, sizeof(data)); // отправляем данные и указываем сколько байт пакет
}
