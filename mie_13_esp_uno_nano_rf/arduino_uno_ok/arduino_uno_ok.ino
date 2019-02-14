
#include <SPI.h>                                            // Подключаем библиотеку для работы с шиной SPI
#include <nRF24L01.h>                                      // Подключаем файл настроек из библиотеки RF24
#include "RF24.h"                                         // Подключаем библиотеку для работы с nRF24L01+
#include "DHT.h"
#define DHTPIN 7  
   
#define DHTTYPE DHT11   // DHT 11

RF24   radio(9, 10);  // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)   
DHT dht(DHTPIN, DHTTYPE); 

void setup(){
    radio.begin();                                        // Инициируем работу nRF24L01+
    radio.setChannel(115); 
    radio.setDataRate     (RF24_1MBPS);    
    radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)//
    radio.openWritingPipe (0x1234567890LL);               // Открываем трубу с идентификатором 0x1234567890 для передачи данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    dht.begin();                                          // Инициируем работу DHT21       
}
void loop(){
  float data[4];   
  delay(2000);  
  float l= analogRead(A1);
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();  
  float s = analogRead(A0);
   
  data[0] = h;  // заполняем массив
  data[1] = t;   
  data[2] = s, DEC; 
  data[3] = l, DEC; 
  radio.write(&data, sizeof(data)); // отправляем данные и указываем сколько байт пакет
 
  }
