/*
   Пример с DS18B20
*/
#define G433_SPEED 1000
#define RADIO_BUF_SIZE 5
#define RADIO_DATA PB0
#define RADIO_VCC PB3
#define DS_DATA PB1
#define DS_VCC PB2

#include <Gyver433.h>
#include <GyverPower.h>
#include <microDS18B20.h>
#include <util/delay.h>

Gyver433_TX <RADIO_DATA, RADIO_BUF_SIZE, G433_XOR> tx;
MicroDS18B20 <DS_DATA> ds;

void setup() {
  pinMode(RADIO_VCC, OUTPUT);         // Пины
  pinMode(DS_VCC, OUTPUT);

  power.hardwareDisable(PWR_ALL);     // сон и периферия
  power.setSleepMode(POWERDOWN_SLEEP);
}

void loop() {
  digitalWrite(DS_VCC, HIGH);   // Подали питание на дс
  _delay_ms(1);                 // Подождем
  ds.requestTemp();             // Запрос температуры
  power.sleep(SLEEP_1024MS);    // Спим и ждем 

  uint16_t result = ds.getRaw(); // проснулись и прочли DS18B20
  uint8_t data[3];              // Формируем и отсылаем пакет
  data[0] = 0xA5;
  data[1] = highByte(result);
  data[2] = lowByte(result);

  digitalWrite(RADIO_VCC, HIGH);
  _delay_ms(1);
  tx.sendData(data);
  digitalWrite(RADIO_VCC, LOW);

  digitalWrite(DS_VCC, LOW);  // Все выключаем и спим
  power.sleepDelay(30000);	  // Спим 30 сек
}
