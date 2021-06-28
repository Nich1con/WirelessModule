/*
   Скетч для базовой станции
*/
#define RADIO_DATA 2
#define RADIO_BUF_SIZE 10
#define G433_SPEED 1000

#include <Gyver433.h>
#include <GyverNTC.h>
#include <microDS18B20.h>

Gyver433_RX <RADIO_DATA, RADIO_BUF_SIZE, G433_XOR> rx;
GyverNTC ntc (0, 10000, 4600, 25, 10000);
MicroDS18B20 <0> ds;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (rx.tickWait()) {      // Ждем пакет по радио
    switch (rx.buffer[0]) { // Получаем адрес модуля
      case 0xA1:            // Кнопка
        Serial.println(rx.buffer[1] ? "Button released" : "Button pressed");
        break;
      case 0xA2:            // Термистор
        Serial.print("NTC: ");
        Serial.print(ntc.computeTemp(rx.buffer[1] << 8 | rx.buffer[2]));
        Serial.println(" *C");
        break;
      case 0xA3:            // Фоторезистор
        Serial.print("Photo value: ");
        Serial.println(rx.buffer[1] << 8 | rx.buffer[2]);
        break;
      case 0xA4:            // PIR детектор
        Serial.println("Pir detected: ");
        break;
      case 0xA5:            // DS18B20
        Serial.print("DS18B20: ");
        Serial.print(ds.calcRaw(rx.buffer[1] << 8 | rx.buffer[2]));
        Serial.println(" *C");
        break;
    }
  }
}
