# MQTT_PowerMeter

## Aktualizacje OTA

Uruchomienie lokalnego serwera OTA

`python ota_server.py build/ 8070 certs/`

Należy ponownie uruchomić urządzenie (ESP32) aby zaktualizować firmware.



## Zmiana wersji firmware

W celu zmienienia numeru wersji oprogramowania, należy zmodyfikować zawartość pliku `version.txt` i ponownie skompilować aplikację.

