/*
   Пример с ПИР
*/

#define G433_SPEED 1000
#define RADIO_BUF_SIZE 5
#define RADIO_DATA PB0
#define RADIO_VCC  PB3
#define PIR_VCC    PB2
#define PIR_DATA   PB1

#include <Gyver433.h>
#include <GyverPower.h>
#include <util/delay.h>

Gyver433_TX <RADIO_DATA, RADIO_BUF_SIZE, G433_XOR> tx;

void setup() {
  pinMode(RADIO_VCC, OUTPUT);
  pinMode(PIR_VCC, OUTPUT);
  pinMode(PIR_DATA, INPUT);

  power.hardwareDisable(PWR_TIMER0);
  power.setSleepMode(POWERDOWN_SLEEP);
}

void loop() {
  digitalWrite(PIR_VCC, HIGH);		// Подаем питание на пир
  power.sleep(SLEEP_512MS);			// Даем датчику время
  
  if (digitalRead(PIR_DATA)) {		// Если кого то спалил
    uint8_t data[2];				// Отправляем пакет
    data[0] = 0xA4;
    data[1] = 0x01;
    digitalWrite(RADIO_VCC, HIGH);	
    _delay_ms(1);
    tx.sendData(data);
    digitalWrite(RADIO_VCC, LOW);
  }
  
  digitalWrite(PIR_VCC, LOW);		// Выкл пир
  power.sleepDelay(30000);			// Спим 30 сек
}
