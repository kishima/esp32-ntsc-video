# esp32-ntsc-video

## confirmed condition

- M5Stack Fire
  - ESP32 + PSRAM 8MB
- ESP-IDF Version: v5.4.1
- latest LovyanGFX (2025/05/31)

## how to use

docker must be executable with user privileges.

### first time

To check if USB device path is correct in `.env` file.

```
./idf.sh build_container
```

### build

```
./idf.sh build
```

### flash

```
./idf.sh flash
```

### monitor

```
./idf.sh monitor
```
