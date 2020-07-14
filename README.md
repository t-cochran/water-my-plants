# water-my-plants
An attempt to water my plants remotely using NodeMCU and Django.

<br>
<img align="right" src="https://i.imgur.com/wbkhrl5.png" width="35%" height="35%">
<img align="right" src="http://clipart-library.com/images_k/plant-transparent-background/plant-transparent-background-7.png" width="20%" height="20%">
<br><br>

**ESP-12E Module Stats:**
- 32-bit RISC microprocessor
- 80 to 160 Mhz clock
- 64kB instruction RAM, 96kB data RAM
- 4 Mbit external flash memory
- IEEE 802.11 WiFi transceiver
- Integrated TCP/IP stack

## Project Goals
- Learn more about the django web framework and cloud deployment models.
- Get more experience with network programming in C.
- ...water my plants while I'm on vacation.

## Setting up the ESP8266 SDK on Arch Linux:
- Install some required packages:
```
sudo pacman -S --needed gcc git make ncurses flex bison gperf python2-pyserial
```
- Download the xtensa toolchain:
```
mkdir -p ~/esp
cd ~/esp
tar -xzf ~/Downloads/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
```
- Download the SDK:
```
cd ~/esp
git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git
```
- Add paths to .bashrc:
```
export PATH="$PATH:$HOME/esp/xtensa-lx106-elf/bin"
export IDF_PATH="$HOME/esp/ESP8266_RTOS_SDK"
```
- Install required python packages:
```
python -m pip install --user -r $IDF_PATH/requirements.txt
```
