

#include <DHT_U.h>
#include <SPI.h>                                           
#include <nRF24L01.h>                                     
#include "RF24.h"                                      
                                   

uint32_t SLEEP_TIME = 30*1000; // Sleep time between reads (in milliseconds) 
   
#define buzzer 3

 RF24   radio(9, 10);  

void setup(){
    radio.begin();                                       
    radio.setChannel(115);                                  
    radio.setDataRate     (RF24_1MBPS);                   // (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS 
    radio.setPALevel      (RF24_PA_HIGH);                 // (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL);      
    tone(buzzer, 300, 500);      
    delay(100); 
    noTone(buzzer); 
}
void loop(){
  float data[5];   
  delay(2000); 

  float h = analogRead(A1);
  float t = analogRead(A0);
  float s = analogRead(A2);
  float l = analogRead(A0);
  float m = analogRead(A0);
   
  data[0] = h;  
  data[1] = t;   
  data[2] = s, DEC; 
  data[3] = l, DEC; 
  data[4] = m, DEC; 

  radio.write(&data, sizeof(data)); 
}
