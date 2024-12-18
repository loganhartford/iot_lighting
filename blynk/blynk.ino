#define BLYNK_TEMPLATE_ID "TMPL2q7VM8qaI"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "0Y7nYHlhuV-Om9fkM4Jy6pHPJbDW00La"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <string.h>

char ssid[] = "LoganWifi";
char pass[] = "logan123";

enum Mode {
  MANUAL,
  DISCO,
  CANDLE,
};

int mode = 0;
int brightness = 0;
int speed = 0;
int red = 0;
int green = 0;
int blue = 0;

// Brightness
BLYNK_WRITE(V0)
{
  brightness = param.asInt(); 

  Serial.print("Brightness is: ");
  Serial.println(brightness);
}

// Mode
BLYNK_WRITE(V1)
{
  mode = param.asInt(); 

  Serial.print("Mode is: ");
  Serial.println(mode);
}

// Speed
BLYNK_WRITE(V3)
{
  speed = param.asInt(); 

  Serial.print("Speed is: ");
  Serial.println(speed);
}

// Red
BLYNK_WRITE(V4)
{
  red = param.asInt(); 

  Serial.print("Red is: ");
  Serial.print(red);
}

// Green
BLYNK_WRITE(V5)
{
  green = param.asInt(); 

  Serial.print(" Green is: ");
  Serial.print(green);
}

// Blue
BLYNK_WRITE(V6)
{
  blue = param.asInt(); 

  Serial.print(" Blue is: ");
  Serial.println(blue);
}


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  WiFi.begin(ssid, pass);

  // Wait for Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    yield();
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  Blynk.run();
  yield();
}
