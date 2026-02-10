#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "time.h"
#include <EEPROM.h>

#define LED_PIN 5
#define LED_COUNT 8
#define GRZYBEK 25

const int EEPROM_SIZE = 512;
const int START_ADDR = 0;

String ssid;
String password;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;      // Polska UTC+1
const int   daylightOffset_sec = 3600; // Zmiana czasu

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int tik = 0;
volatile int pokaz = 20;

// Funkcja zwracajaca kolor dla cyfry 0 - 9
uint32_t resistorColor(int digit) {
  switch (digit) {
    case 0: return strip.Color(0, 0, 0);         // czarny
    case 1: return strip.Color(70, 10, 2);       // brazowy
    case 2: return strip.Color(255, 0, 0);       // czerwony
    case 3: return strip.Color(255, 30, 0);      // pomaranczowy
    case 4: return strip.Color(128, 55, 0);      // zolty
    case 5: return strip.Color(0, 128, 0);       // zielony
    case 6: return strip.Color(0, 0, 128);       // niebieski
    case 7: return strip.Color(64, 0, 74);       // fioletowy
    case 8: return strip.Color(20, 20, 20);      // szary
    case 9: return strip.Color(255, 255, 255);   // bialy
  }
  return strip.Color(0, 0, 0);
}

void handleInterrupt ()
{
  Serial.println("Przerwanie");
  pokaz = 20;
}

void writeStringToEEPROM(int addr, const String &data) {
  int len = data.length();
  EEPROM.write(addr, len);
  for (int i = 0; i < len; i++) {
    EEPROM.write(addr + 1 + i, data[i]);
  }
}

String readStringFromEEPROM(int addr) {
  int len = EEPROM.read(addr);
  if (len <= 0 || len > 250) return "";

  String result = "";
  for (int i = 0; i < len; i++) {
    result += char(EEPROM.read(addr + 1 + i));
  }
  return result;
}

String readSerialWithTrigger() {
  unsigned long start = millis();
  Serial.println("Wcisnij Enter, aby skonfigurowac siec WiFi.");
  while (!Serial.available()) {
    if (millis() - start > 2000) {
      return "";
    }
    delay(5);
  }

  char trigger = Serial.read();

  while (Serial.available()) {
    Serial.read();
  }

  Serial.println("Wpisz dane sieci w formacie:<ssid>\\<haslo>. Masz na to minute");

  start = millis();
  while (!Serial.available()) {
    if (millis() - start > 60000) {
      return "";
    }
    delay(5);
    }

  String input = Serial.readStringUntil('\n');
  input.trim();

  return input;
}


void setup() {
  pinMode(GRZYBEK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(GRZYBEK), handleInterrupt, FALLING);
  
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  String input = readSerialWithTrigger();
  if (input.length() > 0) {
    input.trim();
    Serial.println(input);
    writeStringToEEPROM(START_ADDR, input);
    EEPROM.commit();
  }

  strip.begin();
  strip.show();
  String stored = readStringFromEEPROM(START_ADDR);

  int sep = stored.indexOf('\\');
  if (sep != -1) { 
    ssid = stored.substring(0, sep);
    password = stored.substring(sep + 1);
  }
  Serial.println(ssid);
  Serial.println(password);
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

  // Rozbijanie na cyfry
  int h1 = hour / 10;
  int h2 = hour % 10;

  int m1 = minute / 10;
  int m2 = minute % 10;
  if (pokaz) {
    strip.clear();
  
    strip.setPixelColor(0, resistorColor(h1));
    strip.setPixelColor(2, resistorColor(h2));
  
    if (tik) {
      tik = 0;
      strip.setPixelColor(3, strip.Color(0, 0, 0));
      strip.setPixelColor(4, strip.Color(0, 0, 0));
    } else {
      tik = 1;
      strip.setPixelColor(3, strip.Color(20, 20, 20));
      strip.setPixelColor(4, strip.Color(20, 20, 20));
    }
  
    strip.setPixelColor(5, resistorColor(m1));
    strip.setPixelColor(7, resistorColor(m2));
  
    strip.show();
    pokaz--;
  } else {
    strip.clear();
    strip.show();
  }
  delay(500);
}
