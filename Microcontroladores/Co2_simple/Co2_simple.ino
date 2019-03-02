#include <SoftwareSerial.h>

SoftwareSerial mySerial(A0, A1); // RX, TX


byte cmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};

char response[9];


#define pwmPin 10

int prevVal = LOW;

long th, tl, h, l, ppm, ppm2 = 0.0, ppm3;




void setup() {

    Serial.begin(9600);

    mySerial.begin(9600);

    pinMode(pwmPin, INPUT);

}


void loop(){

    mySerial.write(cmd,9);

    mySerial.readBytes(response, 9);

    int responseHigh = (int) response[2];

    int responseLow = (int) response[3];

    ppm = (256*responseHigh)+responseLow;



    //CO2 via pwm

    do {

        th = pulseIn(pwmPin, HIGH, 1004000) / 1000.0;

        tl = 1004 - th;

        ppm2 = 2000 * (th-2)/(th+tl-4);
        ppm3 =  5000 * (th-2)/(th+tl-4); // расчёт для диапазона от 0 до 5000ppm

    } while (ppm2 < 0.0);

    
     
    
    Serial.print(" <- ppm (UART) ");
    Serial.print((ppm/5)*2);
    Serial.println(ppm);

    Serial.println(ppm2);
    Serial.print(ppm3);
    Serial.println("-----------");

    delay(5000);

}
