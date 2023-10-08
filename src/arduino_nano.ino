#include <stdlib.h>

#include <RH_ASK.h>                              // Подключение библиотеки RadioHead 
#include <SPI.h>                                 // Подключение библиотеки SPI 
#include <iarduino_RTC.h>    
#include <TOTP.h>

#include "DHT.h"

#define DHTTYPE DHT11                           // подключение библиотеки

iarduino_RTC time(RTC_DS1302,6,4,5);
struct dataStructure
{
  char totp[7];
  int temperature;
  int humidity;
};

typedef struct dataStructure Package;
Package data;

uint8_t hmacKey[] = {0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x6b, 0x65, 0x79, 0x30};

TOTP totp = TOTP(hmacKey, 10);
String totpCode = String("");

uint8_t DHTPin = 2; 

RH_ASK rf_driver;                                // Создаем объект "RH_ASK" для работы с библиотекой RadioHead
DHT dht(DHTPin, DHTTYPE);  
void setup()
{
    rf_driver.init();                            // Инициализация библиотеки RadioHead.
    Serial.begin(9600);                          // Настройка последовательной связи
    time.begin();
    dht.begin();
}
 
void loop()
{
    delay(10000); //  задержка в одну секунду перед выводом времени в последовательный пор
    String newCode = String(totp.getCode(time.gettimeUnix()));

    if(totpCode!= newCode)
    {
      totpCode = String(newCode);
      Serial.print("TOTP code: ");
      Serial.println(newCode);
    }
    
    newCode.toCharArray(data.totp, 7);
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();

    rf_driver.send((uint8_t *)&data, sizeof(data));
    rf_driver.waitPacketSent();
}
