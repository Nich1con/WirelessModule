#include <microDS18B20.h>

/*
   Пример нескольких термометров на РАЗНЫХ линиях (пинах) без адресации
   Актуально для 1-2 датчиков, использование адресации сильно экономит память
   Запрос температуры можно производить одновременно для нескольких линий
   Знание уникальных адресов не требуется
   ВНИМАНИЕ! Без адресации на одной линии (пине) не более одного датчика!
*/

/*
  Опрос датчиков асинхронный, т.е. не блокирует выполнение кода, но
  между requestTemp и getTemp должно пройти не менее
  
  точность | время
  12 бит   | 750 мс
  11 бит   | 375 мс
  10 бит   | 187 мс
  9 бит    | 93 мс
  
  Иначе датчик вернёт предыдущее значение
*/

#define SENSOR1_PIN 2   // пины для термометров
#define SENSOR2_PIN 3

MicroDS18B20 <SENSOR1_PIN> sensor1;  // Создаем термометры без адресации
MicroDS18B20 <SENSOR2_PIN> sensor2;

void setup() {
  Serial.begin(9600);
  
  // настройка разрешения (точности) датчика, 9-12 бит. По умолч. 12
  sensor1.setResolution(12);
}

void loop() {
  sensor1.requestTemp();             // Запрашиваем преобразование температуры
  sensor2.requestTemp();             // Сразу на двух датчиках
  delay(1000);                              // Ожидаем окончания преобразования
  Serial.print("Sensor1 temp: ");
  Serial.print(sensor1.getTemp());  // Выводим температуру
  Serial.println(" *C");
  Serial.print("Sensor2 temp: ");
  Serial.print(sensor2.getTemp());
  Serial.println(" *C");
}