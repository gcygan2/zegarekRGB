#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"

#define LED_PIN 5
#define LED_COUNT 8

// WiFi
const char* ssid     = "TP-LINK_EAD700";
const char* password = "Skowronek";

// NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // Polska UTC+1
const int   daylightOffset_sec = 3600; // Zmiana czasu

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Funkcja zwracaj�ca kolor dla cyfry 0�9
uint32_t resistorColor(int digit) {
  switch (digit) {
    case 0: return strip.Color(0, 0, 0);               // czarny
    case 1: return strip.Color(139, 69, 19);           // brazowy
    case 2: return strip.Color(255, 0, 0);             // czerwony
    case 3: return strip.Color(255, 165, 0);           // pomaranczowy
    case 4: return strip.Color(255, 200, 0);           // zolty
    case 5: return strip.Color(0, 255, 0);             // zielony
    case 6: return strip.Color(0, 0, 255);             // niebieski
    case 7: return strip.Color(128, 0, 128);           // fioletowy
    case 8: return strip.Color(128, 128, 128);         // szary
    case 9: return strip.Color(255, 255, 255);         // bialy
  }
  return strip.Color(0, 0, 0);
}

void setup() {
  Serial.begin(115200);

  strip.begin();
  strip.show();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }

  int hour   = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  int second = timeinfo.tm_sec;

  // Rozbijanie na cyfry
  int h1 = hour / 10;
  int h2 = hour % 10;

  int m1 = minute / 10;
  int m2 = minute % 10;

  int s1 = second / 10;
  int s2 = second % 10;

  strip.clear();

  // Mapa LED:
  // 0=h1, 1=h2, 2=spacer, 3=m1, 4=m2, 5=spacer, 6=s1, 7=s2

  strip.setPixelColor(0, resistorColor(h1));
  strip.setPixelColor(1, resistorColor(h2));

  strip.setPixelColor(2, strip.Color(0, 0, 0)); // spacer

  strip.setPixelColor(3, resistorColor(m1));
  strip.setPixelColor(4, resistorColor(m2));

  strip.setPixelColor(5, strip.Color(0, 0, 0)); // spacer

  strip.setPixelColor(6, resistorColor(s1));
  strip.setPixelColor(7, resistorColor(s2));

  strip.show();
  delay(200);
}
   
