#include <RH_ASK.h>                              // Подключение библиотеки RadioHead 
#include <SPI.h>                                 // Подключение библиотеки SPI 
#include <stdlib.h>

struct dataStructure
{
  char totp[7];
  int temperature;
  int humidity;
};

typedef struct dataStructure Package;
Package data;
 
RH_ASK rf_driver;                                // Создаем объект "RH_ASK" для работы с библиотекой RadioHead
 
void setup()
{
    rf_driver.init();                            // Инициализация библиотеки RadioHead.
    Serial.begin(9600);                       // Настройка последовательной связи
    Serial.println("Start");
}
 
void loop()
{
    uint8_t buf[sizeof(data)];
    uint8_t buflen = sizeof(data);
    if (rf_driver.recv(buf, &buflen))           // Включение радиомодуля и чтение сообщения 
    {
      memcpy(&data, &buf, buflen);
        Serial.write(data.totp);
        Serial.write(data.temperature);
        Serial.write(data.humidity);
    }
}
