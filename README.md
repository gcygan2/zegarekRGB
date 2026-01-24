# Zegar RGB (Smartclock)
## Opis
1. Urządzenie zrealizowane na mikrokomputerze z modułem ESP32 wyposażone w interfejs sieci bezprzewodowej (tzw. WiFi). 
2. Pobieranie czasu z serwera NTP.
3. Wyświetlanie czasu na kolorowych diodach LED. Wymagana od użytkownika znajomość standardowego kodu barwnego stosowanego do oznaczania rezystorów.
4. Zasilanie z portu USB.
## Schemat
ESP32 +3.3V ------------------------> VCC WS2812

ESP32 GND --------------------------> GND WS2812

                    330 Ω
ESP32 GPIO 5 ------[    ]-----------> DIN WS2812
    
[HTerm](https://www.der-hammer.info/pages/terminal.html)
