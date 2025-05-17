# MQTT_PowerMeter

[**Dokumentacja projektu**](https://pzpro.atlassian.net/wiki/spaces/PROJEKTOWA/pages/98309/Projekt+MQTT+Power+Meter)

---

## Szybki start

### Wymagania

- Mikrokontroler ESP32 (np. ESP32-C3)
- Czujnik światła LDR
- Komputer z zainstalowanym [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
- Python 3.x
- Dostęp do sieci Wi-Fi oraz serwera z brokerem MQTT (np. Home Assistant + Mosquitto)
- (Opcjonalnie) Docker do uruchomienia Home Assistant, InfluxDB, Grafana, Mosquitto

### Klonowanie repozytorium

```sh
git clone https://github.com/PawelZieba01/MQTT_PowerMeter.git
cd MQTT_PowerMeter
```

### Konfiguracja projektu

1. **Skonfiguruj parametry Wi-Fi i MQTT**  
   Uruchom menu konfiguracyjne ESP-IDF:
   ```sh
   idf.py menuconfig
   ```
   Ustaw:
   - SSID i hasło Wi-Fi (`WiFi Configuration`)
   - Adres brokera MQTT, port, login, hasło (`MQTT Configuration`)
   - Temat publikacji MQTT (`Project Configuration`)
   - Numer GPIO czujnika światła (`Light Sensor Configuration`)

2. **(Opcjonalnie) Skonfiguruj serwer OTA**  
   Jeśli chcesz korzystać z aktualizacji OTA, uruchom lokalny serwer:
   ```sh
   scripts/run_ota_server.cmd
   ```
   Upewnij się, że plik `.bin` z firmware znajduje się w folderze `ota_image/`.

---

## Konfiguracja pliku sdkconfig

Plik `sdkconfig` (generowany przez `idf.py menuconfig`) zawiera wszystkie ustawienia projektu. Najważniejsze opcje konfiguracyjne:

- **WiFi Configuration**  
  - `ESP_WIFI_SSID` – nazwa sieci Wi-Fi, do której łączy się ESP32  
  - `ESP_WIFI_PASSWORD` – hasło do sieci Wi-Fi  
  - `ESP_MAXIMUM_RETRY` – liczba prób połączenia z Wi-Fi

- **MQTT Configuration**  
  - `MQTT_BROKER_URL` – adres brokera MQTT (np. `192.168.0.100`)  
  - `MQTT_BROKER_PORT` – port brokera MQTT (domyślnie `1883`)  
  - `MQTT_BROKER_USERNAME` – login do brokera MQTT  
  - `MQTT_BROKER_PASSWORD` – hasło do brokera MQTT  
  - `MQTT_PUBLISH_INTERVAL` – interwał publikacji danych (w sekundach)

- **OTA Configuration**  
  - `FIRMWARE_UPG_URL` – adres URL serwera OTA z plikiem `.bin`  
  - `OTA_RECV_TIMEOUT` – timeout pobierania aktualizacji (ms)  
  - `OTA_UPDATE_SKIP` – pomiń sprawdzanie aktualizacji OTA

- **Light Sensor Configuration**  
  - `SENSOR_INPUT_PIN` – numer GPIO do którego podłączony jest czujnik światła  
  - `SENSOR_PIN_POLARITY` – polaryzacja impulsu (0 = niski, 1 = wysoki)

- **Project Configuration**  
  - `PROJECT_MQTT_PUB_TOPIC` – główny temat MQTT, do którego publikowane są dane (np. `mqtt/power_meter/`)

Aby zmienić te ustawienia, uruchom:
```sh
idf.py menuconfig
```
i przejdź do odpowiednich sekcji.

---

## Aktualizacje OTA

### Server OTA

Uruchomienie lokalnego serwera OTA

`scripts/run_ota_server.cmd`

Należy ponownie uruchomić urządzenie (ESP32) aby zaktualizować firmware. ESP32 sprawdza dostępność aktualizacji tylko raz po uruchomieniu.

### Aktualizacja wersji firmware

W celu zmienienia numeru wersji oprogramowania, należy zmodyfikować zawartość pliku `version.txt` i ponownie skompilować aplikację przy użyciu polecenia:

`idf.py build -DCOPY_BINARIES=ON`

---

## Budowanie projektu

Aby skompilować projekt, należy użyć polecenia:

`idf.py build -DCOPY_BINARIES=<ON/OFF>`

Flaga `-DCOPY_BINARIES` decyduje czy plik binarny `.bin` jest kopiowany po kompilacji, do folderu `ota_image/`  
Domyślnie flaga jest ustawiona na `OFF`, ale po zmianie jej wartości, trzeba uruchomić `clean project` aby przywrócić domyślne ustawienie. Podczas buildowania projektu najlepiej zawsze podawać wartość flagi `-DCOPY_BINARIES`.

W folderze `ota_image/` powinien znajdować się tylko jeden plik binarny z aktualną wersją firmware'u.

---

## Komponenty i biblioteki

Komponenty i biblioteki powinny znajdować się w `libs/` w osobnych folderach. Należy pamiętać o odpowiedniej konfiguracji w plikach `CMakeLists.txt` dla każdego komponentu/biblioteki.

---

## Dokumentacja

- Szczegółowa dokumentacja, diagramy i opis działania:  
  [https://pzpro.atlassian.net/wiki/spaces/PROJEKTOWA/pages/98309/Projekt+MQTT+Power+Meter](https://pzpro.atlassian.net/wiki/spaces/PROJEKTOWA/pages/98309/Projekt+MQTT+Power+Meter)

---

## Struktura projektu

- `main/` – główny kod aplikacji
- `libs/` – biblioteki: WiFi, OTA, MQTT, czujnik światła
- `ota_image/` – pliki binarne do aktualizacji OTA
- `scripts/` – skrypty pomocnicze (np. serwer OTA)
- `tests/` – testy automatyczne

