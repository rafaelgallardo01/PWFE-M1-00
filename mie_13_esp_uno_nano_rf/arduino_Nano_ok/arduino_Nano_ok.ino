#include <SPI.h>                                            // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                      // Подключаем файл настроек из библиотеки RF24
#include "RF24.h"                                         // Подключаем библиотеку для работы с nRF24L01+

uint32_t SLEEP_TIME = 30*1000; // Sleep time between reads (in milliseconds) 
   


 RF24   radio(9, 10);  // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)   

void setup(){
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(115);                                  // Указываем канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
}
void loop(){
  float data[4];   
  delay(2000);  
  float h= analogRead(A0);
  float t = analogRead(A1);
  float s = analogRead(A0);
  float l = analogRead(A1);
   
  data[0] = h;  // заполняем массив
  data[1] = t;   
  data[2] = s, DEC; 
  data[3] = l, DEC; 
  radio.write(&data, sizeof(data)); // отправляем данные и указываем сколько байт пакет
}
