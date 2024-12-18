#define BLYNK_TEMPLATE_ID "TMPL2q7VM8qaI"
#define BLYNK_TEMPLATE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "0Y7nYHlhuV-Om9fkM4Jy6pHPJbDW00La"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "LoganWifi";
char pass[] = "logan123";

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA); // Set ESP8266 to station mode
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80); // Use manual connection
  WiFi.begin(ssid, pass);

  // Wait for Wi-Fi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
}

void loop() {
  Blynk.run();
  yield(); // Ensure system tasks are handled
}
