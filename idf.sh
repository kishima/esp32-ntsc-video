#!/bin/bash

set -e

source .env

command=$1

#BUILD_CONTAINER=espressif/idf:$IDF_VER
BUILD_CONTAINER=custom_esp_build:$IDF_VER
ESP_TARGET=esp32

case "$command" in
  build_container)
    docker build --build-arg IDF_VER=$IDF_VER -t $BUILD_CONTAINER -f build.dockerfile .
    ;;
  set-target)
    docker run -it --rm -v $PWD/$TARGET_PROJECT:/project -w /project -u $(id -u $USER):$(id -g $USER) -e HOME=/tmp $BUILD_CONTAINER idf.py set-target $ESP_TARGET
    ;;
  menuconfig)
    docker run -it --rm -v $PWD/$TARGET_PROJECT:/project -w /project -u $(id -u $USER):$(id -g $USER) -e HOME=/tmp $BUILD_CONTAINER idf.py menuconfig
    ;;
  clean)
    rm -r $PWD/$TARGET_PROJECT/build
    ;;
  build)
    docker run --rm \
      -e MY_ENV_VAR="$MY_ENV_VAR" \
      -v $PWD/$TARGET_PROJECT:/project -w /project -u $(id -u $USER):$(id -g $USER) -e HOME=/tmp $BUILD_CONTAINER idf.py build
    ;;
  work)
    docker run -it --rm --device=$HOST_USBDEV:/dev/ttyUSB0 -v $PWD/$TARGET_PROJECT:/project -w /project -e HOME=/tmp $BUILD_CONTAINER bash
    ;;
  flash)
    #This command is copied from idf build commnad
    # python -m esptool --chip esp32 -b 460800 --before default_reset --after hard_reset write_flash --flash_mode dio --flash_size 16MB --flash_freq 40m 0x1000 build/bootloader/bootloader.bin 0x8000 build/partition_table/partition-table.bin 0x10000 build/hello_world.bin
    cmd="idf.py flash"
    docker run --rm --device=$HOST_USBDEV:/dev/ttyUSB0 -v $PWD/$TARGET_PROJECT:/project -w /project -e HOME=/tmp $BUILD_CONTAINER bash -c "$cmd"
    ;;
  monitor)
    docker run -it --rm --device=$HOST_USBDEV:/dev/ttyUSB0 -v $PWD/$TARGET_PROJECT:/project -w /project -e HOME=/tmp $BUILD_CONTAINER idf.py monitor
    ;;
  *)
    echo "Command not supported"
    exit 1
    ;;
esac
