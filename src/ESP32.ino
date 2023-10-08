#include <WiFi.h> 
#include <HTTPClient.h> 
#include <TOTP.h>
#include <NTPClient.h>
String URL = "http://computer_IP/projectfolder/file.php";

const char* ssid = "ssid"; 
const char* password = "pswd"; 

uint8_t hmacKey[] = {0x73, 0x65, 0x63, 0x72, 0x65, 0x74, 0x6b, 0x65, 0x79, 0x30}; //пример секретного ключа

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
TOTP totp = TOTP(hmacKey, 10);
String totpCode = String("");

struct dataS
{
  char totp[7];
  int temperature;
  int humidity;
};

typedef struct dataS Package;
Package data;

void setup() 
{
  Serial.begin(9600); 
  connectWiFi();
  timeClient.begin();
  Serial.println("NTP client started");
  Serial.println();
}

void loop() {
    delay(10000);
    timeClient.update();
    
    String newCode = String(totp.getCode(timeClient.getEpochTime() + 10800));

    if(totpCode!= newCode)
    {
      totpCode = String(newCode);
      Serial.print("TOTP code: ");
      Serial.println(newCode);
    }
    char s;
    char buf_this[7];
    char buf[7];
    newCode.toCharArray(buf_this, 7);
    for (int j = 0; j < 6; ++j)
    {
      s = Serial.read();
      buf[j] = s;
    }
    buf[6] = '\0';
    data.temperature = Serial.read();
    data.humidity = Serial.read();

    if (strcmp(buf, buf_this) == 0)
    {
      String postData = "temperature=" + String(data.temperature) + "&humidity=" + String(data.humidity);

      HTTPClient http; 
      http.begin(URL);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
      int httpCode = http.POST(postData); 
      String payload = http.getString(); 
    }
  }

void connectWiFi() 
{
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
}

