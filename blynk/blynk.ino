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
  CYCLE,
  DISCO,
  CANDLE,
};

#include <Adafruit_NeoPixel.h>
#define LED_PIN 4
#define NUM_LEDS 16
#define ESP_LED 2

Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);

int mode = 0;
uint8_t brightness = 0;
int speed = 5;
uint8_t red = 0;
uint8_t green = 0;
uint8_t blue = 0;
int colorIndex = 0;

bool connected = false;
#define CONNECT_THRESH 20

// Brightness
BLYNK_WRITE(V0)
{
  brightness = (uint8_t)param.asInt();
  brightness = brightness * 25.5;

  Serial.print("Brightness is: ");
  Serial.println(brightness);
}

// Mode
BLYNK_WRITE(V1)
{
  mode = param.asInt() - 1; 

  Serial.print("Mode is: ");
  Serial.println(mode);
}

// Speed
BLYNK_WRITE(V3)
{
  speed = 10 - param.asInt() + 1; 

  Serial.print("Speed is: ");
  Serial.println(speed);
}

// Red
BLYNK_WRITE(V4)
{
  red = (uint8_t)param.asInt(); 

  Serial.print("Red is: ");
  Serial.print(red);
}

// Green
BLYNK_WRITE(V5)
{
  green = (uint8_t)param.asInt(); 

  Serial.print(" Green is: ");
  Serial.print(green);
}

// Blue
BLYNK_WRITE(V6)
{
  blue = (uint8_t)param.asInt(); 

  Serial.print(" Blue is: ");
  Serial.println(blue);
}

void updateRingColor() {
  switch (mode) {
    case MANUAL:
      // Static manual color
      for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, ring.Color(green, red, blue));
      }
      break;

    case DISCO:
      // Disco mode: Rainbow animation (each LED different)
      for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, ring.ColorHSV((colorIndex + (i * 65536 / NUM_LEDS)) % 65536, 255, brightness));
      }
      colorIndex = (colorIndex + 256) % 65536; // Increment hue
      break;

    case CANDLE: {
      static float smoothBrightness = brightness;
      
      // Define flicker range as a fraction of current brightness
      float flickerRange = brightness * 0.2; // 20% of brightness
      
      // Ensure some minimum flicker when brightness is low
      if (flickerRange < 10) {
        flickerRange = 10;
      }
      
      // Calculate min/max target
      float minTarget = brightness - flickerRange;
      float maxTarget = brightness + flickerRange;
      
      // Clamp to valid brightness range
      if (minTarget < 0)   minTarget = 0;
      if (maxTarget > 255) maxTarget = 255;
      
      // Pick a random target within flicker range
      float target = random((int)minTarget, (int)maxTarget + 1);
      
      // Smooth transition
      smoothBrightness = smoothBrightness * 0.9 + target * 0.1;
      
      // Set candle color (adjust as desired)
      for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, 95, 180, 0); 
      }
      ring.setBrightness((uint8_t)smoothBrightness);
      break;
    }

    case CYCLE:
      // Cycle mode: All LEDs same color, cycling through RGB spectrum
      uint32_t cycleColor = ring.ColorHSV(colorIndex, 255, brightness);
      for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, cycleColor);
      }
      colorIndex = (colorIndex + 256) % 65536; // Increment hue
      break;
  }
  if (mode != CANDLE)
  {
    ring.setBrightness(brightness);
  }
  ring.show();
}



void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ESP_LED, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  WiFi.begin(ssid, pass);

  // Wait for Wi-Fi to connect
  int connect_count = 0;
  while ((WiFi.status() != WL_CONNECTED) && (connect_count < CONNECT_THRESH)) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, (connect_count % 2));
    connect_count++;
    yield();
  }

  if (connect_count < CONNECT_THRESH) {
    Serial.println("\nConnected to Wi-Fi");
    Blynk.syncAll();

    connected = true;
    mode = CANDLE;
    brightness = 50; // 0-255
    speed = 5;

    // Off
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(ESP_LED, HIGH);
  }
  else {
    Serial.println("\nFailed to connect to Wi-Fi, entering default mode.");

    connected = false;
    mode = CANDLE;
    brightness = 50; // 0-255
    speed = 5;
    
    // One on
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(ESP_LED, LOW);
  }

  ring.begin();
  ring.show();
}

void loop() {
  if (connected)
  {
    Blynk.run();
  }

  updateRingColor();
  if (mode == CANDLE)
  {
    if (random(10)==0)
    {
      delay(random(60, 100));
    }
    else
    {
      delay(random(120,200));
    }
  }
  else
  {
    delay(5*speed);
  }
  yield();
}
