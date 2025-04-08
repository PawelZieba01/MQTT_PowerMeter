# MQTT_PowerMeter

## Aktualizacje OTA

### Server OTA

Uruchomienie lokalnego serwera OTA

`scripts/rub_ota_server.cmd`

Należy ponownie uruchomić urządzenie (ESP32) aby zaktualizować firmware. ESP32 sprawdza dostępność aktualizacji tylko raz po uruchomieniu.



### Aktualizacja wersji firmware

W celu zmienienia numeru wersji oprogramowania, należy zmodyfikować zawartość pliku `version.txt` i ponownie skompilować aplikację przy użyciu polecenia:

`idf.py build -DCOPY_BINARIES=ON`



## Budowanie projektu

Aby skompilować projekt, należy użyć polecenia:

`idf.py build -DCOPY_BINARIES=<ON/OFF>`

Flaga `-DCOPY_BINARIES` decyduje czy plik binarny `.bin` jest kopiowany po kompilacji, do folderu `ota_image/`    
Domyslnie flaga jest ustawiona na `OFF`, ale po zmianie jej wartości, trzeba uruchomić `clean project` aby przywrócić domyślne ustawienie. Podczas buildowania projektu najlepiej zawsze podawać wartość flagi `-DCOPY_BINARIES`.

W folderze `ota_image/` powinien znajdować się tylko jeden plik binarny z aktualną wersją firmware'u.



## Komponenty i biblioteki

Komponenty i biblioteki powinny znajdować się w `libs/` w osobnych folderach. Należy pamiętać o odpowiedniej konfiguracji w plikach `CMakeLists.txt` dla każdego komponentu/biblioteki.




