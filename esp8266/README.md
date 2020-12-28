## Toolchain
Currently, the project only works with old toolchain, `v2.2.0`. To install the toolchain, run the following commands:

```bash
cd ~
git clone https://github.com/esp8266/Arduino.git ~/.esp8266
cd ~/.esp8266
git checkout tags/2.2.0
git submodule update --init
cd tools
python2 get.py # or just python if you have only python2 installed
```

## Build

Just run `make` in the current folder.

## Upload via UART

Just run `make flash` in the current folder.

## Upload via WiFi (OTA)

If you want to upload the firmware via WiFi (OTA), connect ESP8266 and you PC to the same network, then run `make ota`.

In case it can't resolve ESP's IP address, you can hardcode it in the Makefile. Change the following line:
```
 ESP_ADDR ?= esp-trollbooth.local
```
to something like:
```
 ESP_ADDR ?= 192.168.1.170
```
