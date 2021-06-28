/*
   Пример с кнопкой / механическим контактом с подтяжкой
*/
#define G433_SPEED 1000
#define RADIO_BUF_SIZE 5
#define RADIO_DATA PB0
#define RADIO_VCC  PB3

#include <Gyver433.h>
#include <GyverPower.h>
#include <util/delay.h>

Gyver433_TX <RADIO_DATA, RADIO_BUF_SIZE, G433_XOR> tx;

void setup() {
  pinMode(RADIO_VCC, OUTPUT);           // Питание радио
  pinMode(PB1, INPUT_PULLUP);           // Кнопка с подтяжкой

  power.hardwareDisable(PWR_ALL);       // Отключаем всю периферию
  power.setSleepMode(POWERDOWN_SLEEP);  // Самый глубокий сон

  // Для тини 13
  /*
    PCMSK |= 1 << PCINT1;
    GIMSK |= 1 << PCIE;
  */

  // Для меги328
  attachInterrupt(0, wakeUp, LOW);      // Внешнее прерывание
}

void loop() {
  power.sleep(SLEEP_FOREVER);           // Уходим в сон
}

// Для тини 13
/*
  ISR(PCINT0_vect) {
  wakeUp();
  }
*/

void wakeUp() {
  volatile uint8_t data[2];             // Буфер
  data[0] = 0xA1;                       // Адрес
  data[1] = (uint8_t) digitalRead(PB1); // Состояние кнопки

  digitalWrite(RADIO_VCC, HIGH);        // Запитываем
  _delay_ms(1);                         // Немного ждем
  tx.sendData(data);                    // Отправляем пакет
  digitalWrite(RADIO_VCC, LOW);         // Отключаем и идем спать
}
