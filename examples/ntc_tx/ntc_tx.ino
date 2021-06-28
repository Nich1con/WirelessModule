/*
   Пример с АЦП (NTC или фотодатчик)
*/

#define G433_SPEED 1000
#define RADIO_BUF_SIZE 5
#define RADIO_DATA PB0
#define RADIO_VCC  PB3
#define ADC_PIN    A1
#define ADC_VCC    PB1

#include <Gyver433.h>
#include <GyverPower.h>
#include <util/delay.h>

Gyver433_TX <RADIO_DATA, RADIO_BUF_SIZE, G433_XOR> tx;

void setup() {
  pinMode(RADIO_VCC, OUTPUT);
  pinMode(ADC_VCC, OUTPUT);

  power.hardwareDisable(PWR_TIMER0);
  power.setSleepMode(POWERDOWN_SLEEP);
}

void loop() {
  digitalWrite(ADC_VCC, HIGH);			  // Подали питание на цепь термистора / фоторезтора
  uint16_t result = analogRead(ADC_PIN);  // Проснулись и замерили
  digitalWrite(ADC_VCC, LOW);			  // Сняли питание
  
  uint8_t data[3];                // Формируем пакет              
  data[0] = 0xA2;                 // Адрес (А3 для фоторезистора)
  data[1] = highByte(result);     // Старший байт
  data[2] = lowByte(result);      // Младший байт

  digitalWrite(RADIO_VCC, HIGH);  // Вкл радио
  _delay_ms(1);                   // Ждем 
  tx.sendData(data);              // Отправляем
  digitalWrite(RADIO_VCC, LOW);   // Выкл радио

  power.sleepDelay(30000);			// Спим 30 сек
}
